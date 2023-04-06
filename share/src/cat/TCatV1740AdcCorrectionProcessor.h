/* $Id:$ */
/**
 * @file   TCatV1740AdcCorrectionProcessor.h
 * @date   Created : Mar 10, 2013 11:10:09 JST
 *   Last Modified : Mar 18, 2013 14:30:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATV1740ADCCORRECTIONPROCESSOR_H
#define TCATV1740ADCCORRECTIONPROCESSOR_H

#include <TCatProcessor.h>

class TCatV1740AdcCorrectionProcessor  : public TCatProcessor {

public:
   TCatV1740AdcCorrectionProcessor();
   ~TCatV1740AdcCorrectionProcessor();

   void Init(TCatEventCollection*col);
   void Process();

private:
   TClonesArray *fCatHit;
   TClonesArray *fCatHitOut;
   StringVec_t   fInputColName;
   TString       fOutputColName; 
   Int_t         fDoRejectInvalid;

   ClassDef(TCatV1740AdcCorrectionProcessor,1);

};
#endif // end of #ifdef TCATV1740ADCCORRECTIONPROCESSOR_H
