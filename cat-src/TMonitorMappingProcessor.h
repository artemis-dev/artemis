/* $Id:$ */
/**
 * @file   TMonitorMappingProcessor.h
 * @date   Created : Jul 15, 2014 03:40:58 JST
 *   Last Modified : Jul 15, 2014 03:42:26 JST
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TMONITORMAPPINGPROCESSOR_H
#define TMONITORMAPPINGPROCESSOR_H

#include <TProcessor.h>

class TClonesArray;


namespace art {
   class TCategorizedData;
   class TMonitorMappingProcessor;
}

class art::TMonitorMappingProcessor  : public art::TProcessor {

public:
   TMonitorMappingProcessor();
   ~TMonitorMappingProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString     fOutputColName;
   TCategorizedData **fCategorized; //!
   TClonesArray        *fMoniHit;
   Int_t       fCatID;
   
   ClassDef(TMonitorMappingProcessor,1)
};
#endif // end of #ifdef TMONITORMAPPINGPROCESSOR_H
