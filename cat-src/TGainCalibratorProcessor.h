/* $Id:$ */
/**
 * @file   TGainCalibratorProcessor.h
 * @date   Created : May 29, 2015 13:29:49 JST
 *   Last Modified : 2015-05-29 14:23:10 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#ifndef TGAINCALIBRATORPROCESSOR_H
#define TGAINCALIBRATORPROCESSOR_H

#include <TProcessor.h>
#include <TClonesArray.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TGainCalibratorProcessor;
}

class art::TGainCalibratorProcessor  : public TProcessor {

public:
   TGainCalibratorProcessor();
   virtual ~TGainCalibratorProcessor();

   TGainCalibratorProcessor& operator=(const TGainCalibratorProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString      fInputColName;
   TString      fGainConverterArrayName;
   TClonesArray **fHits; //!
   TClonesArray **fGainConverterArray; //!
                      
   Int_t        fEvt;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   FILE         *fp; //!->
   TString      fLogFilePath;
   TString      fLogFileName;
   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;

   ClassDef(TGainCalibratorProcessor,1); // calibrate gain
};
#endif // end of #ifdef TGAINCALIBRATORPROCESSOR_H
