/* $Id:$ */
/**
 * @file   TModuleDecoder.cc
 * @date   Created : Jul 20, 2013 11:20:55 JST
 *   Last Modified : Apr 29, 2014 16:43:22 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoder.h"

#include <TSegmentedData.h>
#include <TRawDataObject.h>
#include <TClass.h>

ClassImp(art::TModuleDecoder)

art::TModuleDecoder::TModuleDecoder()
: fID(0), fHits(NULL)
{
}

art::TModuleDecoder::TModuleDecoder(Int_t id, TClass *dataClass)
   : fID(id), fHits(NULL)
{
   SetClass(dataClass);
}
art::TModuleDecoder::~TModuleDecoder()
{
   if (fHits) delete fHits;
}


void art::TModuleDecoder::SetClass(TClass *dataClass)
{
   // A data class is not indicated, do nothing
   if (!dataClass) return;

   if (!dataClass->InheritsFrom(TRawDataObject::Class())) {
      printf("Decoder[%d] : %s does not inherit from TRawDataObject \n",
             fID,dataClass->GetName());
   }
   if (!fHits) fHits = new TClonesArray(dataClass);
}
