/* $Id:$ */
/**
 * @file   TCatCategorizingProcessor.cc
 * @date   Created : May 13, 2012 19:13:45 JST
 *   Last Modified : Oct 02, 2012 18:43:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCategorizingProcessor.h"

#include <TArtMapper.h>


TCatCategorizingProcessor::TCatCategorizingProcessor()
{
   StringVec_t defInput;
   defInput.push_back("rawdata");
   
   RegisterInputCollection("InputCollection",
                           "rawdata object returned by TArtEventStore",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","categorized rawdata object",
                            fOutputColName,TString("categorized"));
   RegisterProcessorParameter("MapConfig","configuration file to map",
                              fMapFileName,TString("mapper.conf"));
}
TCatCategorizingProcessor::~TCatCategorizingProcessor()
{
   if (fMapper) delete fMapper;
   if (fRawCategrized) delete fRawCategrized;
}


void TCatCategorizingProcessor::Init(TCatEventCollection *col)
{
   fRawData = dynamic_cast<TArtRawEventObject*> 
      (col->Get(fInputColName.front()));
   fRawCategrized = new TCatRawDataCategorized;
   fRawCategrized->SetName(fOutputColName);
   col->Add(fRawCategrized,fOutputIsTransparent);

   // mapper config
   fMapper = TArtMapper::Instance();
   fMapper->LoadConfig(fMapFileName);
}

void TCatCategorizingProcessor::Process()
{
   if (!fRawData) return;

   // Since Actual mapping process is done during parsing RIDF format
   // by TArtParserRIDF for now, here is no mapping procedure
   // implemented. Just use mapped values.
   fRawCategrized->Clear("C");
//   fRawCategrized->Clear();
   // if mapper configuration is not loaded, do nothing
   if (!fMapper->isLoaded()) return;

   // mapping to reference array
   Int_t nSeg = fRawData->GetNumSeg();
   for (Int_t iSeg = 0; iSeg != nSeg; iSeg++) {
      TArtRawSegmentObject *seg = fRawData->GetSegment(iSeg);
      Int_t nData = seg->GetNumData();
      for (Int_t iData = 0; iData < nData; iData++) {
         TArtRawDataObject *obj = (TArtRawDataObject*) seg->GetData(iData);
         const Int_t &cid = obj->GetCategoryID();
         const Int_t &did = obj->GetDetectorID();
         const Int_t &tid = obj->GetDatatypeID();
         if (cid == -9999) continue;
         TObjArray *cat = fRawCategrized->GetCategoryById(cid);
         if (!cat) {
            cat = fRawCategrized->AddCat();
            cat->SetUniqueID(cid);
         }
         
         TObjArray *det = NULL;
         const Int_t &ndet = cat->GetEntriesFast();
         for (Int_t idet=0; idet!=ndet; idet++) {
            if ((Int_t)cat->At(idet)->GetUniqueID() == did) {
               det = (TObjArray*) cat->At(idet);
               break;
            }
         }
         if (!det) {
            det = fRawCategrized->AddDet();
            det->SetUniqueID(did);
            cat->Add(det);
         }
         TRefArray *typ = NULL;
         if ((typ = (TRefArray*)det->At(tid-1))==NULL) {
            typ = fRawCategrized->AddType();
            typ->SetUniqueID(tid);
            det->AddAtAndExpand(typ,tid-1);
         }
         typ->Add(obj);
      }
   }
}
