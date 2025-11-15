/*
 * @file   TSRPPACPlaneDqdxProcessor.h
 * @brief  divide SRPPAC TimingChargeData into collection for each plane
 *
 * @date   Created:       2019-08-30 19:28:42 JST (ota)
 *         Last Modified: 2019-08-30 20:18:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2019 Shinsuke OTA
 */

#ifndef TSRPPACPLANEDQDXPROCESSORV2_H
#define TSRPPACPLANEDQDXPROCESSORV2_H

#include <TProcessor.h>
#include "TSRPPACPlaneProcessorV2.h"

#include <vector>

namespace art {
   class TSRPPACPlaneDqdxProcessorV2;
   class TConverterBase;
}

class TClonesArray;

class art::TSRPPACPlaneDqdxProcessorV2 : public TSRPPACPlaneProcessorV2 {
public:
   // Default constructor
   TSRPPACPlaneDqdxProcessorV2();
   virtual ~TSRPPACPlaneDqdxProcessorV2();

   void SetConvCoeff(Double_t val) { fConvCoeff = val; }
   Double_t GetConvCoeff() const { return fConvCoeff; }

   virtual void Process();

protected:
   TConverterBase** fConverter; // converter
   TString	         fConverterName;    // name of dq-to-length converter
   Int_t fCombination;

   Double_t fConvCoeff;

   ClassDef(TSRPPACPlaneDqdxProcessorV2,0) // processor for SRPPAC plane division
};

#endif // TSRPPACPLANDQDXEPROCESSORV2_H
