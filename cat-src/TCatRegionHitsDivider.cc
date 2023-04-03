/* $Id:$ */
/**
 * @file   TCatRegionHitsDivider.cc
 * @date   Created : Aug 31, 2016 15:31:27 JST
 *   Last Modified : 2016-08-31 16:43:45 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#include "TCatRegionHitsDivider.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TEventHeader.h>

using art::TCatRegionHitsDivider;

ClassImp(TCatRegionHitsDivider)

TCatRegionHitsDivider::TCatRegionHitsDivider()
{
  StringVec_t def;
  def.push_back("atpulse");
  RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputRecoilCollection","output pulse data (TCatPulseShape)",
			   fOutputRecoilColName,TString("atrecoilcand"));
  RegisterOutputCollection("OutputBeamCollection","output pulse data (TCatPulseShape)",
			   fOutputBeamColName,TString("atbeamcand"));
  RegisterProcessorParameter("SavePulse","flag to save pulse",
                             fSavePulse,kFALSE);
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("regionhitsdivider.log"));
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
  RegisterInputCollection("EventHeader","name of event header (default : eventheader)",fNameEventHeader,TString("eventheader"),
                          &fEventHeader,art::TEventHeader::Class_Name());
}

TCatRegionHitsDivider::~TCatRegionHitsDivider()
{
  delete fOutputRecoil;
  delete fOutputBeam;
}

TCatRegionHitsDivider::TCatRegionHitsDivider(const TCatRegionHitsDivider& rhs)
{
  MayNotUse("TCatRegionHitsDivider");
}

TCatRegionHitsDivider& TCatRegionHitsDivider::operator=(const TCatRegionHitsDivider& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatRegionHitsDivider::Init(TEventCollection *col)
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
  fOutputRecoil = new TClonesArray("art::TCatPulseShape");
  col->Add(fOutputRecoilColName,fOutputRecoil,fOutputIsTransparent);
  fOutputBeam = new TClonesArray("art::TCatPulseShape");
  col->Add(fOutputBeamColName,fOutputBeam,fOutputIsTransparent);
}

void TCatRegionHitsDivider::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   fEvt = (*fEventHeader)->GetEventNumber();
   fOutputRecoil->Clear("C");
   fOutputBeam->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();

   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();
  
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   if (fDetailDebug) if (nHits) printf("-----------\n[regionhitsdivider]   Evt = %6d, nHits = %3d\n",fEvt,nHits);

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit  = (TCatPulseShape*) (input)->At(iHit);
      if (!(hit->IsValid())) continue;
      Int_t id = hit->GetID();

      if ((id>=60) && (id<340)) { // beam region (|x| < 20 mm) hits
         if (fDebug) {
            Float_t off = hit->GetOffset();
            Int_t col = id / 10;
            Int_t row = id % 10;
            if ((id > 69 && id < 330) && (col%2 == 0)) {
               col = col - 1;
               row = row + 10;
            } 
            fprintf(fp,"iHit = %3d, id = %3d, col = %2d, row = %2d, off = %4.0f, Beam_cand\n",iHit,id,col,row,off);
         }
         TCatPulseShape *out = (TCatPulseShape*) fOutputBeam->ConstructedAt(fOutputBeam->GetEntriesFast());
         hit->Copy(*out);
          
         // optional output
         if (fSavePulse) {
            Int_t num = 0;
            num = hit->GetNumSample();
            out->GetSample().resize(num);
            out->GetClock().resize(num);
            std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
            for (Int_t i=0; i != num; i++) {
               out->GetClock()[i] = (Double_t)i;
            }
            out->SetNumSample(num);
         }
      } else { // recoil region (|x| >= 20 mm) hits
         if (fDebug) {
            Float_t off = hit->GetOffset();
            Int_t col = id / 10;
            Int_t row = id % 10;
            if ((id > 69 && id < 330) && (col%2 == 0)) {
               col = col - 1;
               row = row + 10;
            } 
            fprintf(fp,"iHit = %3d, id = %3d, col = %2d, row = %2d, off = %4.0f, Recoil_cand\n",iHit,id,col,row,off);
         }
         TCatPulseShape *out = (TCatPulseShape*) fOutputRecoil->ConstructedAt(fOutputRecoil->GetEntriesFast());
         hit->Copy(*out);
          
         // optional output
         if (fSavePulse) {
            Int_t num = 0;
            num = hit->GetNumSample();
            out->GetSample().resize(num);
            out->GetClock().resize(num);
            std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
            for (Int_t i=0; i != num; i++) {
               out->GetClock()[i] = (Double_t)i;
            }
            out->SetNumSample(num);
         }
      }
   }
}
