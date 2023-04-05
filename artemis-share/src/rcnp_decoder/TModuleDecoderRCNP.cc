/* $Id:$ */
/**
 * @file   TModuleDecoderRCNP.cc
 * @date   Created : Jul 20, 2013 11:20:55 JST
 *   Last Modified : May 12, 2014 23:25:54 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoderRCNP.h"

#include <TSegmentedData.h>
#include <TRawDataObject.h>
#include <TClass.h>

ClassImp(art::TModuleDecoderRCNP)

art::TModuleDecoderRCNP::TModuleDecoderRCNP()
: fID(0), fHits(NULL)
{
}

art::TModuleDecoderRCNP::TModuleDecoderRCNP(Int_t id, TClass *dataClass)
   : fID(id), fHits(NULL)
{
   SetClass(dataClass);
}
art::TModuleDecoderRCNP::~TModuleDecoderRCNP()
{
   if (fHits) delete fHits;
}


void art::TModuleDecoderRCNP::SetClass(TClass *dataClass)
{
   // A data class is not indicated, do nothing
   if (!dataClass) return;
	printf("aa\n");

   if (!dataClass->InheritsFrom(TRawDataObject::Class())) {
      printf("Decoder[%d] : %s does not inherit from TRawDataObject \n",
             fID,dataClass->GetName());
   }
   if (!fHits) fHits = new TClonesArray(dataClass);
}

void art::TModuleDecoderRCNP::Copy(TObject& obj) const
{
   TModuleDecoderRCNP &decoder = static_cast<TModuleDecoderRCNP&>(obj);
   decoder.fID = fID;
}