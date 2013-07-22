/* $Id:$ */
/**
 * @file   TModuleDecoderFactory.h
 * @date   Created : Jul 22, 2013 08:22:10 JST
 *   Last Modified : Jul 22, 2013 08:54:40 JST
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
   ~TModuleDecoderFactory();

   static TModuleDecoderFactory* Instance();

   void Register(TModuleDecoder *decoder);
   TModuleDecoder* Get(Int_t did) { return (TModuleDecoder*) fDecoders->At(did); }

protected:
   TObjArray *fDecoders;

};
#endif // end of #ifdef TMODULEDECODERFACTORY_H
