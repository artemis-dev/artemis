/* $Id:$ */
/**
 * @file   TRandomNumberEventStore.h
 * @date   Created : Jul 11, 2013 18:11:07 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRANDOMNUMBEREVENTSTORE_H
#define TRANDOMNUMBEREVENTSTORE_H

#include <TProcessor.h>
#include <TConditionBit.h>

#include <TSimpleData.h>

namespace art {

   class TRandomNumberEventStore;
}


class art::TRandomNumberEventStore  : public TProcessor {

public:
   TRandomNumberEventStore();
   ~TRandomNumberEventStore();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   
private:
   Int_t           fMaxLoop;
   Float_t        fMin;
   Float_t        fMax;
   TSimpleData    *fData;
   TString         fOutputColName;

   Int_t           fNumLoop; //! local variable

   ClassDef(TRandomNumberEventStore,1);
};

#endif // end of #ifdef TRANDOMNUMBEREVENTSTORE_H
