/**
 * @file   TPPACProcessorQTC.cc
 */

#include "TPPACProcessorQTC.h"
#include "TPPACData.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"
#include "TPPACParameter.h"

#include "constant.h"

#include <TCategorizedData.h>
#include <TRawTiming.h>

#include <TClonesArray.h>

using art::TPPACProcessorQTC;

ClassImp(art::TPPACProcessorQTC)

// Default constructor
TPPACProcessorQTC::TPPACProcessorQTC() 
   : fPPACOut(NULL), 
     fTimingConverterArray(NULL), fChargeConverterArray(NULL),
     fXConverterArray(NULL), fYConverterArray(NULL)
{
   StringVec_t defInput(1,"catdata");
   RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			   fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
			    fOutputColName,TString("ppac"));
   RegisterProcessorParameter("CatID","Category ID of PPAC",fCatID,1);
   RegisterProcessorParameter("TimingConverterArray",
			      "name of time converter array (ch2ns)",
			      fTimingConverterArrayName,TString("ppac_ch2ns"));
   RegisterProcessorParameter("ChargeConverterArray","name of charge converter array (qped)",
			      fChargeConverterArrayName,TString("ppac_qped"));
   RegisterProcessorParameter("XConverterArray","name of x position converter array (ns2mm)",
			      fXConverterArrayName,TString("ppac_xconv"));
   RegisterProcessorParameter("YConverterArray","name of y position converter array (ns2mm)",
			      fYConverterArrayName,TString("ppac_yconv"));
   RegisterOptionalInputInfo("PPACParameter","name of ppac parameter",fParameterName,
                     TString("prm:ppac"),&fParameterSet,"TClonesArray","art::TPPACParameter");
   defInput.clear();
   RegisterOptionalParameter("ListOfParameterNames","list of names of parameter",
                             fListOfParameterNames,defInput);
   RegisterOptionalParameter("ListOfPPACNames",
                             "list of names of ppac: the size should be same as the maximum detector id.\n"
                             "Index corresponds to the id of PPAC.\n"
                             "If name at certain index is empty the output of correnponding ppac will be ommitted.",
                             fListOfPPACNames,defInput);
   RegisterOptionalParameter("DoSeparatePPACs","separate output for each ppacs",
                             fDoSeparatePPACs,kFALSE);
   RegisterOptionalParameter("HasEachChConverter","converter should be prepared for each channel",
                             fHasEachChConverter,kFALSE);
   RegisterOptionalParameter("doInsideOffset","calibrate delayline offset if non 0",fDoInsideOffset,kTRUE);
   RegisterOptionalParameter("doOutsideOffset","calibrate line offset if non 0",fDoOutsideOffset,kTRUE);
   RegisterOptionalParameter("doGeometryOffset","calibrate geometry offset if non 0",fDoGeometryOffset,kTRUE);
   
}


TPPACProcessorQTC::~TPPACProcessorQTC() {
   delete fPPACOut;
   delete fTimingConverterArray;
   delete fChargeConverterArray;
   delete fXConverterArray;
   delete fYConverterArray;
}

void TPPACProcessorQTC::Init(TEventCollection *col) {

   fCategorizedData = reinterpret_cast<TCategorizedData**>(col->GetObjectRef(fInputColName[0].Data()));
   if (!fCategorizedData) return;

   fPPACOut = new TClonesArray(TPPACData::Class());
   fPPACOut->SetName(fOutputColName);
   col->Add(fOutputColName,fPPACOut,fOutputIsTransparent);

   Info("Init","CatID: %d  => %s\n",fCatID,fOutputColName.Data());

   TConverterUtil::SetConverterArray(&fTimingConverterArray,fTimingConverterArrayName,col);
   TConverterUtil::SetConverterArray(&fChargeConverterArray,fChargeConverterArrayName,col);
   TConverterUtil::SetConverterArray(&fXConverterArray,fXConverterArrayName,col);
   TConverterUtil::SetConverterArray(&fYConverterArray,fYConverterArrayName,col);

   if (fDoSeparatePPACs) {
      const Int_t &nPPACs = fListOfPPACNames.size();
      const Int_t &nParams = fListOfParameterNames.size();
      if (!nPPACs) {
         Warning("Init","Cannot separate PPACs since no name is set while DoSeparatePPACs is true");
         return;
      }
      if (!nParams) {
         std::copy(fListOfPPACNames.begin(),fListOfPPACNames.end(),std::back_inserter(fListOfParameterNames));
      } else if (nParams != nPPACs) {
         SetStateError("The numbers of parameters and PPACs are different, while trying to separate PPAC output");
         return;
      }
      fPPACArray.resize(nPPACs,NULL);
      fPPACParameter.resize(nPPACs,NULL);
      for (Int_t i = 0; i!=nPPACs; i++) {
         if (fListOfPPACNames[i].IsNull()) continue;
         fPPACArray[i] = new TClonesArray(TPPACData::Class());
         col->Add(fListOfPPACNames[i],fPPACArray[i],fOutputIsTransparent);
         TPPACParameter *prm = dynamic_cast<TPPACParameter*>((*fParameterSet)->FindObject(fListOfParameterNames[i]));
         if (!prm) {
            SetStateError(TString::Format("No such parameter '%s' is found,while trying to separate PPAC output",
                                          fListOfParameterNames[i].Data()));
            return;
         }
         fPPACParameter[i] = prm;
      }
   }
}

void TPPACProcessorQTC::Process() {
   if (!fCategorizedData) return;

   fPPACOut->Clear("C");
   if (fDoSeparatePPACs) {
      for (std::vector<TClonesArray*>::iterator it = fPPACArray.begin(); it != fPPACArray.end(); it ++) {
         (*it)->Clear("C");
      }
   }

   const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
   if (!cat) return;
   Int_t t[TPPACData::kNRAW], q[TPPACData::kNRAW];

   //printf("\x1b[31mNcat %d\x1b[0m\n", cat->GetEntriesFast());
   const Int_t n = cat->GetEntriesFast();   
   for(Int_t i = 0; i != n ; ++i) {
      const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
      Bool_t idFlag = kFALSE;
      Int_t detID = kInvalidI;

      for (Int_t j = 0; j != TPPACData::kNRAW ; ++j) {

	 t[j] = q[j] = kInvalidI;

     //printf("Ndet %d\n", det->GetEntriesFast());
	 const TObjArray *const tArray = static_cast<TObjArray*>(det->At(j));
	 //const TObjArray *const qArray = static_cast<TObjArray*>(det->At(j+TPPACData::kNRAW));

	 if(tArray && tArray->GetEntriesFast() ) {
	    TRawTiming *const tHit1 = static_cast<TRawTiming*>(tArray->At(0));
	    TRawTiming *const tHit2 = static_cast<TRawTiming*>(tArray->At(1));
        //printf("NtArray %d\n", tArray->GetEntriesFast());
        //printf("%s\n", tHit1->GetName());
        //tHit1->Dump();
	    if (!tHit1 || !tHit2) continue;
	    t[j] = tHit1->GetTiming();
	    if( !idFlag ) {
	       detID = tHit1->GetDetID();
	       idFlag = IsValid(detID);
	    }
        q[j] = tHit2->GetTiming() - t[j];
	 }

      }

      if (!idFlag) continue;

      TPPACData *const ppac = static_cast<TPPACData*>(fPPACOut->ConstructedAt(fPPACOut->GetEntriesFast()));
      ppac->SetDetID(detID);
      for (Int_t j = 0; j != TPPACData::kNRAW ; ++j) {
	 if ( IsValid(t[j]) ) {	    
	    if (fTimingConverterArray) {
               TConverterBase *conv = NULL;
               if (fHasEachChConverter) {
                  conv = fTimingConverterArray->at(detID * TPPACData::kNRAW + j);
               } else {
                  conv = fTimingConverterArray->at(detID);
               }
               if (conv) {
                  ppac->SetT(conv->Convert(t[j]), j);
               } else {
                  ppac->SetT(t[j],j);
               }
	    } else {
	       ppac->SetT(t[j],j);
	    }
	 }
	 
	 if ( IsValid(q[j]) ) {
         //printf("valid q");
	    if (fChargeConverterArray) {
               TConverterBase *conv = NULL;
               if (fHasEachChConverter) {
                  conv = fChargeConverterArray->at(detID * TPPACData::kNRAW + j);
               } else {
                  conv = fChargeConverterArray->at(detID);
               }
	       ppac->SetQ(conv->Convert(q[j]), j);
	    } else {
	       ppac->SetQ(q[j],j);
	    }
	 }
      }
      if (fXConverterArray) {
	 ppac->SetXConverter(fXConverterArray->at(detID - DETID_ORIGIN));
      }
      if (fYConverterArray) {
	 ppac->SetYConverter(fYConverterArray->at(detID - DETID_ORIGIN));
      }
      ppac->Update();
      if (fDoSeparatePPACs && detID < (Int_t)fPPACArray.size()) {
         Double_t X, Y;
         TClonesArray *arr = fPPACArray[detID];
         TPPACData* ppacdata = (TPPACData*) arr->ConstructedAt(arr->GetEntriesFast());
         TPPACParameter *prm = (TPPACParameter*) fPPACParameter[detID];
         ppac->Copy(*ppacdata);
         Double_t dTX = ppac->GetTX1() - ppac->GetTX2();
         Double_t dTY = ppac->GetTY1() - ppac->GetTY2();

         // if (delaylinecalibrated)
         if (fDoInsideOffset) {
            dTX += prm->GetInsideOffset(0);
            dTY += prm->GetInsideOffset(1);
         }

         // if (line calib ofset)
         if (fDoOutsideOffset) {
            dTX -= prm->GetOutsideOffset(0);
            dTY -= prm->GetOutsideOffset(1);
         }
         
         ppacdata->SetTXDiff(dTX);
         ppacdata->SetTYDiff(dTY);
         // exchange and calibration
         X = dTX * prm->GetNs2mm(0) * 0.5;
         Y = dTY * prm->GetNs2mm(1) * 0.5;
         if (prm->GetExchangeXY()) {
            Double_t tmp = X;
            X = Y;
            Y = tmp;
         }
         if (prm->GetReflection()) {
            X *= -1;
         }

         // geometry offset
         if (fDoGeometryOffset) {
            X -= prm->GetGeometryOffset(0);
            Y -= prm->GetGeometryOffset(1);
         }
         ppacdata->ResetQualityBit(TDataObject::kInvalid);
         if (!prm->IsInsideTXSum(ppacdata->GetTXSum())) {
            ppacdata->SetQualityBit(TPPACData::kBadTXSum | TDataObject::kInvalid);
         }
         if (!prm->IsInsideTYSum(ppacdata->GetTYSum())) {
            ppacdata->SetQualityBit(TPPACData::kBadTYSum | TDataObject::kInvalid);
         }
         ppacdata->SetX(X);
         ppacdata->SetY(Y);
         ppacdata->SetZ(prm->GetGeometryOffset(2));
         ppacdata->Copy(*ppac);
         
      }
   }
}

