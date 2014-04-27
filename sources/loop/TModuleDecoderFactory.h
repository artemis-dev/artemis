/* $Id:$ */
/**
 * @file   TModuleDecoderFactory.h
 * @date   Created : Jul 22, 2013 08:22:10 JST
 *   Last Modified : Apr 27, 2014 12:43:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODERFACTORY_H
#define TMODULEDECODERFACTORY_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderFactory;
}

class art::TModuleDecoderFactory  {

protected:
   TModuleDecoderFactory();

public:
   virtual ~TModuleDecoderFactory();

   // singleton for register the decoders
   static TModuleDecoderFactory* Instance();

   // clone this instance for the multiple use of TRIDFEventStore
   TModuleDecoderFactory* CloneInstance();

   void Register(TModuleDecoder *decoder);
   TModuleDecoder* Get(Int_t did) { return (TModuleDecoder*) fDecoders->At(did); }

   virtual void Clear() {
      Int_t n = fDecodersSparse->GetEntriesFast();
      for (Int_t i=0; i!=n; i++) {
         fDecodersSparse->At(i)->Clear("C");
      }
   }

protected:
   TObjArray *fDecoders;
   TObjArray *fDecodersSparse;

   ClassDef(TModuleDecoderFactory,1); // module decoder factory
};
#endif // end of #ifdef TMODULEDECODERFACTORY_H
