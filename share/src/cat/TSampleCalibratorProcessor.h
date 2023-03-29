/* $Id:$ */
/**
 * @file   TSampleCalibratorProcessor.h
 * @date   Created : Feb 20, 2014 08:20:05 JST
 *   Last Modified : 2018-02-04 11:49:20 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TSAMPLECALIBRATORPROCESSOR_H
#define TSAMPLECALIBRATORPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TSampleCalibratorProcessor;
}

class TClonesArray;


class art::TSampleCalibratorProcessor  : public TProcessor {

public:
   TSampleCalibratorProcessor();
   virtual ~TSampleCalibratorProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString     fADCConverterArrayName;
   TClonesArray **fHits; //!
   TClonesArray **fADCConverterArray; //!
                      
   ClassDef(TSampleCalibratorProcessor,1) // calibrate sampling 
};
#endif // end of #ifdef TSAMPLECALIBRATORPROCESSOR_H
