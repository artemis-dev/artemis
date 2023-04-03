/* $Id:$ */
/**
 * @file   TSampleCalibratorTester.h
 * @date   Created : 2015-03-24 16:26:52 JST
 *   Last Modified : 2015-05-19 17:16:32 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TSampleCalibratorProcessor.h
 *  
 *    Copyright (C)2015
 */
#ifndef TSAMPLECALIBRATORTESTER_H
#define TSAMPLECALIBRATORTESTER_H

#include <TProcessor.h>
#include <TClonesArray.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TSampleCalibratorTester;
}


class art::TSampleCalibratorTester  : public TProcessor {

public:
   TSampleCalibratorTester();
   virtual ~TSampleCalibratorTester();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString     fADCConverterArrayName;
   TClonesArray **fHits; //!
   TClonesArray **fADCConverterArray; //!
                      
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

   ClassDef(TSampleCalibratorTester,1); // calibrate sampling 
};
#endif // end of #ifdef TSAMPLECALIBRATORTESTER_H
