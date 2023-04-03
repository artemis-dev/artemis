/* $Id:$ */
/**
 * @file   TCatAtBeamInRecoilRegionCounter.cc
 * @date   Created : Feb 15, 2016 11:15:16 JST
 *   Last Modified : 2016-02-15 17:19:22 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#include "TCatAtBeamInRecoilRegionCounter.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <stdio.h>
#include <TMath.h>

using art::TCatAtBeamInRecoilRegionCounter;
using art::TCatPulseShape;

ClassImp(TCatAtBeamInRecoilRegionCounter)

TCatAtBeamInRecoilRegionCounter::TCatAtBeamInRecoilRegionCounter()
{
  StringVec_t def;
  def.push_back("atrecoilcls");
  RegisterInputCollection("InputCollection","input pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
			   fOutputColName,TString("atrecoilcls2"));
  RegisterOutputCollection("OutputExcludeEvtCollection","output pulse data (TCatPulseShape)",
			   fOutputExcludeEvtColName,TString("atexcludeevt3"));
  RegisterProcessorParameter("HistSaveFlag","Flag For Histgram Save",fSaveHist,false);
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterProcessorParameter("ExcludeEvtSaveMode","Flag For Saving Excluding Events",fSaveExcludeEvt,false);
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("beaminrecoilregioncounter.log"));
}
TCatAtBeamInRecoilRegionCounter::~TCatAtBeamInRecoilRegionCounter()
{
  delete fOutput;
  if (fSaveExcludeEvt) delete fExcludeEvtOutput;
  if (fSaveHist) {
    delete fhRowRecoilRange;
  }
}
TCatAtBeamInRecoilRegionCounter::TCatAtBeamInRecoilRegionCounter(const TCatAtBeamInRecoilRegionCounter& rhs)
{
  MayNotUse("TCatAtBeamInRecoilRegionCounter");
}
TCatAtBeamInRecoilRegionCounter& TCatAtBeamInRecoilRegionCounter::operator=(const TCatAtBeamInRecoilRegionCounter& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatAtBeamInRecoilRegionCounter::Init(TEventCollection *col)
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
  if (fSaveExcludeEvt) {
    fExcludeEvtOutput = new TClonesArray("art::TCatPulseShape");
    col->Add(fOutputExcludeEvtColName,fExcludeEvtOutput,fOutputIsTransparent);
  }
  if (fSaveHist) {
     fhRowRecoilRange = new TH1F("hRowRecoilRange","hRowRecoilRange",10,0.5,10.5);
  }
  fEvt = 0;
}

void TCatAtBeamInRecoilRegionCounter::Process()
{
   fOutput->Clear("C");
   if (fSaveExcludeEvt) fExcludeEvtOutput->Clear("C");   
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();
   std::vector<TCatPulseShape*> hits;
   
   if (fDebug) fprintf(fp,"-----------\n*Evt = %d, nHits = %3d\n",fEvt,nHits);
   if (fDetailDebug) printf("[beaminrecoilcounter]   Evt = %6d, nHits = %3d\n",fEvt,nHits);
   
   // Count hits each row in recoil region
   Int_t nHitsLeftRow[10] = {};
   Int_t nHitsRightRow[10] = {};
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit  = (TCatPulseShape*) (input)->At(iHit);
      Int_t id = hit->GetID();
      Int_t col = id / 10;
      Int_t row = id % 10;
      if ((id > 69 && id < 330) && (col%2 == 0)) {
         col = col - 1;
         row = row + 10;
      }
      if (id < 60) {
         nHitsLeftRow[row]++;
         if (fDebug) fprintf(fp,"Left  Recoil Hits: id = %d, off = %4.0f\n",id,hit->GetOffset());
      } else if (id >= 340) {
         nHitsRightRow[row]++;
         if (fDebug) fprintf(fp,"Right Recoil Hits: id = %d, off = %4.0f\n",id,hit->GetOffset());
      }
      hits.push_back(hit);
   }
   nHits = hits.size();
   if (fDebug) fprintf(fp,"nHits = %d\n",nHits);
   /*----------------------------------------------------------------*/

   Int_t nRowLeftRegion = 0;
   Int_t nRowRightRegion = 0;
   for (Int_t iRow=0; iRow<10; iRow++) {
      if (nHitsLeftRow[iRow]) nRowLeftRegion++;
      if (nHitsRightRow[iRow]) nRowRightRegion++;
   }

   Bool_t IsSkip = false;
   if ((nRowLeftRegion >= 7) || (nRowRightRegion >= 7)) {
      IsSkip = true;
      if (fDebug) fprintf(fp,"nRowLeftRegion = %d, nRowRightRegion = %d\n",nRowLeftRegion,nRowRightRegion);
   }
   Int_t nRow = -1;
   nRow = (nRowLeftRegion >= nRowRightRegion) ? nRowLeftRegion : nRowRightRegion;
   if (fSaveHist) fhRowRecoilRange->Fill(nRow);
      
   if (IsSkip) {
      if (fDebug) fprintf(fp,"Evt = %d, nRow = %d, Skip\n",fEvt,nRow);
      //printf("Evt = %d, nRow = %d, Skip\n",fEvt,nRow);
      if (fSaveExcludeEvt) {
         std::vector<TCatPulseShape*>::iterator it = hits.begin();
         std::vector<TCatPulseShape*>::iterator itend = hits.end();
         for (; it != itend; it++) {
            TCatPulseShape *out = (TCatPulseShape*) fExcludeEvtOutput->ConstructedAt(fExcludeEvtOutput->GetEntriesFast());
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
      return;
   }
   /*----------------------------------------------------------------*/

   // Save
   std::vector<TCatPulseShape*>::iterator it = hits.begin();
   std::vector<TCatPulseShape*>::iterator itend = hits.end();
   if (fDebug) fprintf(fp,"Evt = %d, nRow = %d, Skip\n",fEvt,nRow);
   if (fDetailDebug) printf("Evt = %d, nRow = %d, Skip\n",fEvt,nRow);
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
   /*--------------------------------------------------------------------------*/

   fEvt++;
}
