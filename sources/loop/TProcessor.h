/* $Id:$ */
/**
 * @file   TProcessor.h
 * @date   Created : Jul 10, 2013 17:10:49 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TPROCESSOR_H
#define TPROCESSOR_H

#include <TEventCollection.h>

namespace art {
   class TProcessor;
}

class art::TProcessor  {

public:
   TProcessor();
   ~TProcessor();

   virtual void Init (TEventCollection *) {;}
   virtual void BeginOfRun() {;}
   virtual void EndOfRun() {;}
   virtual void Process() {;}
   virtual void PreLoop() {;}
   virtual void PostLoop() {;}

   virtual void Clear(Option_t* /* opt */) {;}
};
#endif // end of #ifdef TPROCESSOR_H
