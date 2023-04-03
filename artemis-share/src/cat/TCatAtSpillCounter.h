/* $Id:$ */
/**
 * @file   TCatAtSpillCounter.h
 * @date   Created : Feb 12, 2013 23:12:55 JST
 *   Last Modified : Feb 13, 2013 00:19:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATSPILLCOUNTER_H
#define TCATATSPILLCOUNTER_H

#include <TProcessor.h>

#include <TCategorizedData.h>

namespace art {
  class TCategorizedData;
  class TCatAtSpillCounter;
}

class art::TCatAtSpillCounter  : public art::TProcessor {

public:
   TCatAtSpillCounter();
   ~TCatAtSpillCounter();

   void Init(art::TEventCollection *);
   void Process();

protected:
   StringVec_t fInputColName;
   TString     fOutputColName;
   TCategorizedData **fCategorized;
   TClonesArray       *fCATHit;
   Int_t       fCatID;

   ClassDef(TCatAtSpillCounter,1);
};
#endif // end of #ifdef TCATATSPILLCOUNTER_H
