/* $Id:$ */
/**
 * @file   TModuleDecoderSIS3610.cc
 * @date   Created : Dec 24, 2011 19:24:19 JST
 *   Last Modified : Oct 31, 2013 06:10:30 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TModuleDecoderSIS3610.h"

#include <TRawDataSimple.h>

using art::TModuleDecoderSIS3610;

ClassImp(art::TModuleDecoderSIS3610)

TModuleDecoderSIS3610::TModuleDecoderSIS3610()
   : TModuleDecoder(kID,TRawDataSimple<UShort_t>::Class()) {
}

TModuleDecoderSIS3610::~TModuleDecoderSIS3610()
{
}


Int_t TModuleDecoderSIS3610::Decode(char* buf, const int &size, TObjArray *seg)
{
   TRawDataSimple<UShort_t>* data = (TRawDataSimple<UShort_t>*)(this->New());
   data->SetSegInfo(seg->GetUniqueID(),0,0);
   data->Set(*(short*)buf);
   seg->Add(data);
   return 0;
}
