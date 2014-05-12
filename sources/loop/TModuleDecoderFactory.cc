/* $Id:$ */
/**
 * @file   TModuleDecoderFactory.cc
 * @date   Created : Jul 22, 2013 08:22:15 JST
 *   Last Modified : May 13, 2014 00:07:13 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoderFactory.h"
#include <TClass.h>

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

art::TModuleDecoderFactory* art::TModuleDecoderFactory::CloneInstance()
{
   TModuleDecoderFactory *factory = new TModuleDecoderFactory;
   Int_t nDec = fDecodersSparse->GetEntriesFast();
   for (Int_t i=0; i!=nDec; i++) {
      TModuleDecoder *olddec = (TModuleDecoder*)fDecodersSparse->At(i);
      TModuleDecoder *newdec = (TModuleDecoder*)olddec->IsA()->New();
      //    new copies the
      olddec->Copy(*newdec);
      factory->Register(newdec);
   }
   return factory;
}

void art::TModuleDecoderFactory::Register(TModuleDecoder *decoder)
{
   if (fDecoders->GetEntriesFast() < decoder->GetID() ||
       !fDecoders->At(decoder->GetID())) {
      fDecoders->AddAtAndExpand(decoder,decoder->GetID());
      fDecodersSparse->Add(decoder);
   } else {
      printf("Module Decoder %s (ID = %d) already registered\n",decoder->Class()->GetName(),decoder->GetID());
   }
}

