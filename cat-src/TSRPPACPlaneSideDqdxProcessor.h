/*
 * @file   TSRPPACPlaneDqdxProcessor.h
 * @brief  divide SRPPAC TimingChargeData into collection for each plane
 *
 * @date   Created:       2019-08-30 19:28:42 JST (ota)
 *         Last Modified: 2019-08-30 20:18:10 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TSRPPACPLANESIDEDQDXPROCESSOR_H
#define TSRPPACPLANESIDEDQDXPROCESSOR_H

#include <TProcessor.h>
#include "TSRPPACPlaneProcessor.h"

#include <vector>

namespace art {
   class TSRPPACPlaneSideDqdxProcessor;
   class TConverterBase;
}

class TClonesArray;

class art::TSRPPACPlaneSideDqdxProcessor : public TSRPPACPlaneProcessor {
public:
   // Default constructor
   TSRPPACPlaneSideDqdxProcessor();
   virtual ~TSRPPACPlaneSideDqdxProcessor();

   virtual void Process();

protected:
   TConverterBase** fConverter1; // converter
   TString	         fConverterName1;    // name of dq-to-length converter
   TConverterBase** fConverter2; // converter
   TString	         fConverterName2;    // name of dq-to-length converter
//   TConverterBase** fConverter3; // converter
//   TString	         fConverterName3;    // name of dq-to-length converter
//   TConverterBase** fConverter4; // converter
//   TString	         fConverterName4;    // name of dq-to-length converter
   Int_t fCombination;

   ClassDef(TSRPPACPlaneSideDqdxProcessor,0) // processor for SRPPAC plane division
};

#endif // TSRPPACPLANDSIDEQDXEPROCESSOR_H
