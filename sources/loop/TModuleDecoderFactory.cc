/* $Id:$ */
/**
 * @file   TModuleDecoderFactory.cc
 * @date   Created : Jul 22, 2013 08:22:15 JST
 *   Last Modified : Oct 18, 2013 16:36:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoderFactory.h"

art::TModuleDecoderFactory::TModuleDecoderFactory()
{
   fDecoders = new TObjArray;
   fDecodersSparse = new TObjArray;
}
art::TModuleDecoderFactory::~TModuleDecoderFactory()
{
   delete fDecoders;
   delete fDecodersSparse;
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
      fDecodersSparse->Add(decoder);
   } else {
      printf("Module Decoder %d already registered\n",decoder->ID());
   }
}

