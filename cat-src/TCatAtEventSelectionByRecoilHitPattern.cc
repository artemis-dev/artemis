/*
 * @file   TCatAtEventSelectionByRecoilHitPattern.cc
 * @brief  event selection for CAT
 *
 * @date   Created       : 2014-03-08 09:32:26 JST
 *         Last Modified : 2016-06-21 07:49:32 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 */

#include "TCatAtEventSelectionByRecoilHitPattern.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <stdio.h>
#include <TMath.h>

using art::TCatAtEventSelectionByRecoilHitPattern;
using art::TCatPulseShape;

ClassImp(TCatAtEventSelectionByRecoilHitPattern)

TCatAtEventSelectionByRecoilHitPattern::TCatAtEventSelectionByRecoilHitPattern()
{
  StringVec_t def;
  def.push_back("athitcls");
  RegisterInputCollection("InputCollection","input pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
			   fOutputColName,TString("atevt"));
  RegisterOutputCollection("OutputExcludeEvtCollection","output pulse data (TCatPulseShape)",
			   fOutputExcludeEvtColName,TString("atexcludeevt"));
  RegisterOutputCollection("OutputExcludeLessEvtCollection","output pulse data (TCatPulseShape)",
			   fOutputExcludeLessEvtColName,TString("atexcludelessevt"));
  RegisterOutputCollection("OutputExcludeMoreEvtCollection","output pulse data (TCatPulseShape)",
			   fOutputExcludeMoreEvtColName,TString("atexcludemoreevt"));
  RegisterProcessorParameter("SetNColRecoilRange","Set # of Column of Recoil Range",
                             fSetNColRecoilRange,2);
  RegisterProcessorParameter("HistSaveFlag","Flag For Histgram Save",fSaveHist,false);
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterProcessorParameter("MinNHits","Min # of Hits",fMinNHits,(Int_t)0);
  RegisterProcessorParameter("ExcludeEvtSaveMode","Flag For Saving Excluding Events",fSaveExcludeEvt,false);
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("evtsel.log"));
}

TCatAtEventSelectionByRecoilHitPattern::~TCatAtEventSelectionByRecoilHitPattern()
{
  delete fOutput;
  if (fSaveCandidatesCls) delete fCandidatesClsOutput;
  if (fSaveExcludeEvt) delete fExcludeEvtOutput;
  if (fSaveExcludeEvt) delete fExcludeLessEvtOutput;
  if (fSaveExcludeEvt) delete fExcludeMoreEvtOutput;
  if (fSaveHist) {
    delete fhColRecoilRange;
    delete fhNumOfGoodEvent;
  }
}

TCatAtEventSelectionByRecoilHitPattern::TCatAtEventSelectionByRecoilHitPattern(const TCatAtEventSelectionByRecoilHitPattern& rhs)
{
  MayNotUse("TCatAtEventSelectionByRecoilHitPattern");
}

TCatAtEventSelectionByRecoilHitPattern& TCatAtEventSelectionByRecoilHitPattern::operator=(const TCatAtEventSelectionByRecoilHitPattern& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatAtEventSelectionByRecoilHitPattern::Init(TEventCollection *col)
{
  TEventObject *obj = NULL;
  if (!(obj = col->Get(fInputColName.front()))) {
    Info("Init","No such input %s",fInputColName.front().Data());
    return;
  }
  // Prepare Log File
  if (fDebug || fDetailDebug) {
     const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
     fp = fopen(LogNamePath,"w");
     if (fp == NULL) {
        Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
        exit(1);
     }
  }
  fInput = (TClonesArray**) obj->GetObjectRef();
  fOutput = new TClonesArray("art::TCatPulseShape");
  col->Add(fOutputColName,fOutput,fOutputIsTransparent);
  if (fSaveCandidatesCls) {
    fCandidatesClsOutput = new TClonesArray("art::TCatPulseShape");
    col->Add("atcandcls",fCandidatesClsOutput,fOutputIsTransparent);
  }
  if (fSaveExcludeEvt) {
    fExcludeEvtOutput = new TClonesArray("art::TCatPulseShape");
    col->Add(fOutputExcludeEvtColName,fExcludeEvtOutput,fOutputIsTransparent);
    fExcludeLessEvtOutput = new TClonesArray("art::TCatPulseShape");
    col->Add(fOutputExcludeLessEvtColName,fExcludeLessEvtOutput,fOutputIsTransparent);
    fExcludeMoreEvtOutput = new TClonesArray("art::TCatPulseShape");
    col->Add(fOutputExcludeMoreEvtColName,fExcludeMoreEvtOutput,fOutputIsTransparent);
  }
  if (fSaveHist) {
     fhColRecoilRange = new TH1F("hColRecoilRange","hColRecoilRange",7,0.,7.);
     fhNumOfGoodEvent = new TH1F("hNumOfGoodEvent","hNumOfGoodEvent",2,0.,2.);
  }
  fEvt = 0;
}

void TCatAtEventSelectionByRecoilHitPattern::Process()
{
   fOutput->Clear("C");
   if (fSaveCandidatesCls) fCandidatesClsOutput->Clear("C");
   if (fSaveExcludeEvt) fExcludeEvtOutput->Clear("C");   
   if (fSaveExcludeEvt) fExcludeLessEvtOutput->Clear("C");   
   if (fSaveExcludeEvt) fExcludeMoreEvtOutput->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();
   std::vector<TCatPulseShape*> hits;
   std::vector<TCatPulseShape*> recoilhits;
   Bool_t IsGood = false;

   if (fDebug) fprintf(fp,"-----------\n*Evt = %d, nHits = %3d\n",fEvt,nHits);
   if (fDetailDebug) printf("[evetselc]   Evt = %6d, nHits = %3d\n",fEvt,nHits);

   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit  = (TCatPulseShape*) (input)->At(iHit);
      if (!(hit->IsValid())) continue;
      Int_t id = hit->GetID();
      if (id>=400) continue;
      hits.push_back(hit);
      if (id<60 || id>=340) recoilhits.push_back(hit);
      
   }
   Int_t nRecoilHits = recoilhits.size();
   if (fDebug) fprintf(fp,"nRecoilHits = %d\n",nRecoilHits);
   if (fDetailDebug) printf("nRecoilHits = %d\n",nRecoilHits);
   /*----------------------------------------------------------------*/

   std::vector<TCatPulseShape*>::iterator recoilit1 = recoilhits.begin();
   std::vector<TCatPulseShape*>::iterator recoilit1end = recoilhits.end();
   std::vector<TCatPulseShape*>::iterator recoilit2 = recoilhits.begin();
   std::vector<TCatPulseShape*>::iterator recoilit2end = recoilhits.end();
   std::vector<TCatPulseShape*>::iterator recoilit_tmp = recoilhits.begin();
   Int_t nNeighborHits = 0;
   Bool_t IsNeighbor[nRecoilHits][nRecoilHits];
   for (Int_t i=0; i<nRecoilHits; i++) {
      for (Int_t j=0; j<nRecoilHits; j++) {
         IsNeighbor[i][j] = false;
      }
   }
   std::vector<TCatPulseShape*> neighborhits;
   for (; recoilit1 != recoilit1end; recoilit1++) {
      Int_t id2,id1 = id2 = (*recoilit1)->GetID();
      Double_t x2,x1 = x2 = (*recoilit1)->GetX();
      Double_t z2,z1 = z2 = (*recoilit1)->GetZ();
      Float_t off2,off1 = off2 = (*recoilit1)->GetOffset();

      recoilit2 = recoilhits.begin();
      for (; recoilit2 != recoilit2end; recoilit2++) {
         if (recoilit1>=recoilit2) continue;
         if (fDebug) fprintf(fp,"i = %zd, id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",std::distance(recoilhits.begin(),recoilit1),id1,x1,z1,off1);
         if (fDetailDebug) printf("i = %zd, id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",std::distance(recoilhits.begin(),recoilit1),id1,x1,z1,off1);
         id2 = (*recoilit2)->GetID();
         x2 = (*recoilit2)->GetX();
         z2 = (*recoilit2)->GetZ();
         off2 = (*recoilit2)->GetOffset();
         if (fDebug) fprintf(fp,"j = %zd, id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",std::distance(recoilhits.begin(),recoilit2),id2,x2,z2,off2);
         if (fDetailDebug) printf("j = %zd, id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",std::distance(recoilhits.begin(),recoilit2),id2,x2,z2,off2);

         Double_t distance = TMath::Sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2));
         Float_t  diffoff  = TMath::Abs(TMath::Abs(off1) - TMath::Abs(off2));
         if (fDebug) fprintf(fp,"distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);
         if (fDetailDebug) printf("distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);
         
         if ((TMath::Abs(distance) < 13.) && (TMath::Abs(diffoff) <= 20.)) {
            nNeighborHits++;
            neighborhits.push_back(*recoilit1);
            neighborhits.push_back(*recoilit2);
            IsNeighbor[std::distance(recoilhits.begin(),recoilit1)][std::distance(recoilhits.begin(),recoilit2)] = true;
            if (recoilit_tmp==recoilhits.begin()) recoilit_tmp = recoilit1;
         } else if (TMath::Abs(diffoff) > 20.) {
            if (fDebug) fprintf(fp,"TMath::Abs(diffoff) > 20.\n");
            break;
         }
      }
   }
   
   nNeighborHits = 0;
   Int_t nNeighborHits_tmp = 0;
   Int_t iTmp = -10000;
   Int_t jTmp = 0;
   for (Int_t i=0; i<nRecoilHits; i++) {
      Int_t j = 0;
      if (iTmp!=-10000) {
         j = jTmp;
      } else {
         j = 0;
      }
      for (; j<nRecoilHits; j++) {
         if (i>=j) continue;
         if (fDebug) fprintf(fp,"(i,j) = (%d,%d)\n",i,j);
         if (IsNeighbor[i][j]) {
            nNeighborHits++;
            if ((iTmp+1==i) || (iTmp==-10000)) {
               iTmp = i-1; 
               jTmp = j+1;
            }
            i = j-1;
            if (fDebug) fprintf(fp,"IsNeighbor. (i,j) = (%d,%d), (iTmp+1,jTmp) = (%d,%d)\n",
                                i,j,iTmp+1,jTmp);
            break;
         }
      }
      if (j==nRecoilHits) { // not found 
         if ((iTmp+1!=i) && (iTmp!=-10000)) i = iTmp;
         if (iTmp+1==i) iTmp = -10000;
         if (fDebug) fprintf(fp,"iTmp+1 = %d, jTmp = %d\n",iTmp+1,jTmp);
         if (nNeighborHits_tmp < nNeighborHits) nNeighborHits_tmp = nNeighborHits;
         nNeighborHits = 0;
         if (fDebug) fprintf(fp,"nNeighborHits reset\n");
         continue;
      }
      if (fDebug) fprintf(fp,"(i,j) = (%d,%d) end\n",i,j);
   }
   nNeighborHits = nNeighborHits_tmp;
   if (fDebug) fprintf(fp,"nNeighborHits = %d\n",nNeighborHits);
   if (fDetailDebug) printf("nNeighborHits = %d\n",nNeighborHits);

   if (nNeighborHits >= 5) IsGood = true;
   if (IsGood && fDebug) fprintf(fp,"IsGood = %d\n",IsGood);
   if (IsGood && fDetailDebug) printf("IsGood = %d\n",IsGood);
   if (fDetailDebug) printf("IsGood = %d\n",IsGood);

   /*----------------------------------------------------------------*/

   // Save
   if (IsGood){
      std::vector<TCatPulseShape*>::iterator it = hits.begin();
      std::vector<TCatPulseShape*>::iterator itend = hits.end();
      //if (fDebug) fprintf(fp,"Evt = %d, nHits = %d, nNeighborHits = %d\n",fEvt,nHits,nNeighborHits);
      //if (fDetailDebug) printf("Evt = %d, nHits = %d, nNeighborHits = %d\n",fEvt,nHits,nNeighborHits);
      for (; it != itend; it++) {
         Int_t id = (*it)->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
           
         TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
         (*it)->Copy(*out);
           
         Int_t num = 0;
         num = (*it)->GetNumSample();
         out->GetSample().resize(num);
         out->GetClock().resize(num);
         std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
         for (Int_t i=0; i != num; i++) {
            out->GetClock()[i] = (Double_t)i;
         }
         out->SetNumSample(num);
      }
   }

   fEvt++;
}
