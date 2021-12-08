/* $Id:$ */
/**
 * @file   TRandomNumberEventStore.h
 * @date   Created : Jul 11, 2013 18:11:07 JST
 *   Last Modified : 2021-12-08 13:57:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRANDOMNUMBEREVENTSTORE_H
#define TRANDOMNUMBEREVENTSTORE_H

#include "TProcessor.h"
#include "TConditionBit.h"
#include "IEventStore.h"

#include "TSimpleData.h"

namespace art {

   class TRandomNumberEventStore;
}


class art::TRandomNumberEventStore  : public TProcessor, public IEventStore {

public:
   TRandomNumberEventStore();
   ~TRandomNumberEventStore();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   
private:
   Int_t           fNumLoop; //! local variable
   Int_t           fMaxLoop;
   Float_t        fMin;
   Float_t        fMax;
   TSimpleData    *fData;
   TString         fOutputColName;


   ClassDef(TRandomNumberEventStore,1);
};

#endif // end of #ifdef TRANDOMNUMBEREVENTSTORE_H
