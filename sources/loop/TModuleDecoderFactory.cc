/* $Id:$ */
/**
 * @file   TModuleDecoderFactory.cc
 * @date   Created : Jul 22, 2013 08:22:15 JST
 *   Last Modified : Jul 22, 2013 08:48:47 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoderFactory.h"

art::TModuleDecoderFactory::TModuleDecoderFactory()
{
   fDecoders = new TObjArray;
}
art::TModuleDecoderFactory::~TModuleDecoderFactory()
{
   delete fDecoders;
}

art::TModuleDecoderFactory* art::TModuleDecoderFactory::Instance()
{
   static TModuleDecoderFactory instance;
   return &instance;
}

void art::TModuleDecoderFactory::Register(TModuleDecoder *decoder)
{
   if (fDecoders->GetEntriesFast() < decoder->ID() ||
       !fDecoders->At(decoder->ID())) {
      fDecoders->AddAtAndExpand(decoder,decoder->ID());
   } else {
      printf("Module Decoder %d already registered\n",decoder->ID());
   }
}

