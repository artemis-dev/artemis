
/* $Id:$ */
/**
 * @file   TLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : Mar 20, 2013 18:03:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TLOOP_H
#define ART_TLOOP_H


#include <list>

#include <TConditionBit.h>
#include <TEventCollection.h>


namespace art {
   class TLoop;
   class TProcessor;
};

class art::TLoop : public TObject {

public:
   /*
    * enumeration for conditions 
    */
   enum EProc {kRunning, kBeginOfRun, kEndOfRun, kStopEvent, kStopLoop};
   static const char* kConditionName;


   TLoop();
   ~TLoop();


   Bool_t Init();
   Bool_t Resume();

   virtual void        Clear(Option_t * /*option*/ ="") { }

   Bool_t Load(const char* filename);
   TConditionBit *GetCondition() { return fCondition; }

private:
   TConditionBit *fCondition;
   
   TEventCollection   *fEventCollection;
   std::list<TProcessor*>   fProcessors;

   ClassDef(TLoop,1);
};


#endif // end of #ifdef TLOOP_H
