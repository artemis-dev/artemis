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

#ifndef TSRPPACPLANEDQDXPROCESSOR_H
#define TSRPPACPLANEDQDXPROCESSOR_H

#include <TProcessor.h>
#include "TSRPPACPlaneProcessor.h"

#include <vector>

namespace art {
   class TSRPPACPlaneDqdxProcessor;
   class TConverterBase;
}

class TClonesArray;

class art::TSRPPACPlaneDqdxProcessor : public TSRPPACPlaneProcessor {
public:
   // Default constructor
   TSRPPACPlaneDqdxProcessor();
   virtual ~TSRPPACPlaneDqdxProcessor();

   virtual void Process();

protected:
   TConverterBase** fConverter; // converter
   TString	         fConverterName;    // name of dq-to-length converter
   Int_t fCombination;

   ClassDef(TSRPPACPlaneDqdxProcessor,0) // processor for SRPPAC plane division
};

#endif // TSRPPACPLANDQDXEPROCESSOR_H
