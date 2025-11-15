/* $Id:$ */
/**
 * @file   TGainCalibratorProcessor.cc
 * @date   Created : May 29, 2015 13:29:30 JST
 *   Last Modified : 2015-05-29 15:29:19 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#include "TGainCalibratorProcessor.h"
#include <TCatPulseShape.h>
#include <TConverterBase.h>
#include <stdlib.h>

using art::TGainCalibratorProcessor;

ClassImp(TGainCalibratorProcessor)

TGainCalibratorProcessor::TGainCalibratorProcessor()
   : fHits(NULL),fGainConverterArray(NULL),
     fEvt(0), fRunHeaders(NULL), fRunInfo(NULL), fRunName(""), fRunNumber(0)
{
   RegisterInputCollection("InputCollection","input collection",fInputColName,TString("atpulse"),
                           &fHits,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterProcessorParameter("GainConverterArrayName","converter array name for Gain",fGainConverterArrayName,TString(""));
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("gaincalibproc.log"));
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
}

TGainCalibratorProcessor::~TGainCalibratorProcessor()
{
}

TGainCalibratorProcessor& TGainCalibratorProcessor::operator=(const TGainCalibratorProcessor& rhs)
{
   if (this != &rhs) {
   }
   return *this;
}

void TGainCalibratorProcessor::Init(TEventCollection *col)
{
   fGainConverterArray = (TClonesArray**) col->GetObjectRef(fGainConverterArrayName);
   if (!fGainConverterArray) {
      Error("Init","No such converter '%s'",fGainConverterArrayName.Data());
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
}

void TGainCalibratorProcessor::Process()
{
   if (!fHits) return;
   if (!fGainConverterArray) return;

   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }

   Int_t nHits = (*fHits)->GetEntriesFast();
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fHits)->At(iHit);
      if (!(pulse->IsValid())) {
         if (fDetailDebug) printf("[gaincalib] Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,pulse->GetID());
         if (fDebug) fprintf(fp,"Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,pulse->GetID());
         break;
      }
      Double_t charge = pulse->GetCharge();
      TConverterBase* converter = (TConverterBase*) (*fGainConverterArray)->At(pulse->GetID());
      if (converter) pulse->SetCharge(converter->Convert(charge));
      if (fDebug) fprintf(fp,"iHit = %3d, id = %3d, charge = %0.2f -> %0.2f\n",iHit,pulse->GetID(),charge,converter->Convert(charge));
   }
   fEvt++;
}
