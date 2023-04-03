/* $Id:$ */
/**
 * @file   TSampleCalibratorTester.cc
 * @date   Created : 2015-03-24 16:27:50 JST
 *   Last Modified : 2015-04-29 16:04:29 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TSampleCalibratorProcessor.cc
 *  
 *    Copyright (C)2015
 */
#include "TSampleCalibratorTester.h"
#include <TCatPulseShape.h>
#include <TConverterBase.h>
#include <vector>
#include <stdlib.h>

using art::TSampleCalibratorTester;

ClassImp(TSampleCalibratorTester);

TSampleCalibratorTester::TSampleCalibratorTester()
   : fHits(NULL),fADCConverterArray(NULL),
     fEvt(0), fRunHeaders(NULL), fRunInfo(NULL), fRunName(""), fRunNumber(0)
{
   StringVec_t input;
   input.push_back("at");
   RegisterInputCollection("InputCollection","input collection",fInputColName,input);
   RegisterProcessorParameter("ADCConverterArrayName","converter array name for ADC",fADCConverterArrayName,
                              TString(""));
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("samplecalibtester.log"));
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
}
TSampleCalibratorTester::~TSampleCalibratorTester()
{
   if (fDebug || fDetailDebug) fclose(fp);
}

void TSampleCalibratorTester::Init(TEventCollection *col)
{
   fHits = (TClonesArray**)  col->GetObjectRef(fInputColName[0]);
   if (!fHits) {
      Error("Init","No such input '%s'",fInputColName[0].Data());
      return;
   }
   fADCConverterArray = (TClonesArray**) col->GetObjectRef(fADCConverterArrayName);
   if (!fADCConverterArray) {
      Error("Init","No such converter '%s'",fADCConverterArrayName.Data());
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
   
void TSampleCalibratorTester::Process()
{
   if (!fHits) return;
   if (!fADCConverterArray) return;

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
         if (fDetailDebug) printf("[samplecalib] Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,pulse->GetID());
         if (fDebug) fprintf(fp,"Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,pulse->GetID());
         break;
      }
      std::vector<Float_t>& sample = pulse->GetSample();
      Int_t nSample = pulse->GetNumSample();
      TConverterBase* converter = (TConverterBase*) (*fADCConverterArray)->At(pulse->GetGeo()*64+pulse->GetCh());
      if (converter) {
         for (Int_t i=0; i!=nSample; i++) {
            sample[i] = converter->Convert(sample[i]);
         }
      }
   }
   fEvt++;
}
