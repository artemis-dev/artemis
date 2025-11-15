/* $Id:$ */
/**
 * @file   TPulseUnfoldHighPassTester.cc
 * @date   Created : Jan 31, 2014 13:31:12 JST
 *   Last Modified : 2015-04-29 16:05:18 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TPulseUnfoldHighPassProcessor.cc
 *  
 *    Copyright (C)2015
 */
#include "TPulseUnfoldHighPassTester.h"
#include <TCatPulseShape.h>

#include <vector>
#include <algorithm>
#include <numeric>

ClassImp(art::TPulseUnfoldHighPassTester);

using art::TPulseUnfoldHighPassTester;

TPulseUnfoldHighPassTester::TPulseUnfoldHighPassTester()
   : fEvt(0), fRunHeaders(NULL), fRunInfo(NULL), fRunName(""), fRunNumber(0)
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse shape (TCatPulseShape)",
                           fInputColName,def);
   RegisterProcessorParameter("TimeConstant","time constant of amplifier which will be used for pole zero canceleration",
                              fTimeConstant,(Float_t)(16./20.));
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("polezerotester.log"));
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
}
TPulseUnfoldHighPassTester::~TPulseUnfoldHighPassTester()
{
   if (fDebug || fDetailDebug) fclose(fp);
}

void TPulseUnfoldHighPassTester::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   Info("Init","Input %s (overwritten)",fInputColName.front().Data());
   Info("Init","TimeConstant = %f",fTimeConstant);

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

void TPulseUnfoldHighPassTester::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   Int_t nHits = (*fInput)->GetEntries();
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   const Float_t kM = 1./(fTimeConstant+1);
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)(*fInput)->At(iHit);
      if (!(hit->IsValid())) {
         if (fDetailDebug) printf("[unfoldhighpass] Evt = %d, iHit = %d, id = %d, Invalid hit\n",fEvt,iHit,hit->GetID());
         if (fDebug) fprintf(fp,"Evt = %d, iHit = %d, id = %d, Invalid hit\n",fEvt,iHit,hit->GetID());
         break;
      }
      std::vector<Float_t>& sample = hit->GetSample();
      std::vector<Float_t> sum(sample.size());
      std::partial_sum(sample.begin(),sample.end(),sum.begin());
      std::transform(sample.begin(),sample.end(),sample.begin(),
                     std::bind(std::multiplies<Float_t>(),std::placeholders::_1,fTimeConstant));
      std::transform(sample.begin(),sample.end(),sum.begin(),sample.begin(),std::plus<Float_t>());
      std::transform(sample.begin(),sample.end(),sample.begin(),std::bind(std::multiplies<Float_t>(),std::placeholders::_1,kM));
      hit->SetQualityBit(TCatPulseShape::kHighPassUnfolded);
   }
   fEvt++;
}
