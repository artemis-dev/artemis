/* $Id:$ */
/**
 * @file   TCatReadoutPadConditionProcessor.h
 * @date   Created : Mar 20, 2013 20:20:37 JST
 *   Last Modified : 2017-12-21 05:15:17 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATREADOUTPADCONDITIONPROCESSOR_H
#define TCATREADOUTPADCONDITIONPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCatReadoutPadConditionProcessor;
   class TCatReadoutPadArray;
}

class art::TCatReadoutPadConditionProcessor  : public TProcessor {

public:
   TCatReadoutPadConditionProcessor();
   virtual ~TCatReadoutPadConditionProcessor();

   virtual void Init(TEventCollection *evt);

   virtual void SetPadType(Int_t type) { fPadType = type; }
   virtual TCatReadoutPadArray* GetPadArray() { return fPads; }

protected:
   virtual void Construct();
   virtual void Construct01();
   virtual void Construct02();
   virtual void Construct_M01();
   virtual void Construct_Mini();

protected:
   Int_t fPadType;  // type of pad configuration
   Int_t fNumPads; // number of pads
   TString fName; // name of output
   TCatReadoutPadArray *fPads;  //!->

   IntVec_t fBads; // bad channels
   IntVec_t fLowGains; // low gain channels

   Double_t fGap; // full width of gap between two pads
   
   ClassDef(TCatReadoutPadConditionProcessor,1); // pad condition processor
};
#endif // end of #ifdef TCATREADOUTPADCONDITIONPROCESSOR_H
