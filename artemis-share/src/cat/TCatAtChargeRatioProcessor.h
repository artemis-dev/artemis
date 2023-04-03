/* $Id:$ */
/**
 * @file   TCatAtChargeRatioProcessor.h
 * @date   Created : Oct 26, 2013 02:26:59 JST
 *   Last Modified : Oct 26, 2013 02:58:47 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATCHARGERATIOPROCESSOR_H
#define TCATATCHARGERATIOPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCatAtChargeRatioProcessor;
}

class art::TCatAtChargeRatioProcessor  : public art::TProcessor {

public:
   TCatAtChargeRatioProcessor();
   virtual ~TCatAtChargeRatioProcessor();


   virtual Init(TEventCollection *col);
   virtual Process();

private:
   TClonesArray *fCatHits;
   TClonesArray *fPosition;

   TString fInputColName;
   TString fOutputColName;
};
#endif // end of #ifdef TCATATCHARGERATIOPROCESSOR_H
