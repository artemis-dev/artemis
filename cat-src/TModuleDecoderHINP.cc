/**
 * @file   TModuleDecoderHINP.cc
 * @brief  module decoder for HINP
 *
 * @date   Created       : 2015-10-04 16:52:03 JST
 *         Last Modified : 2015-10-04 18:41:47 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TModuleDecoderHINP.h"
#include <TRawDataTimingCharge.h>

using art::TModuleDecoderHINP;

ClassImp(TModuleDecoderHINP)

TModuleDecoderHINP::TModuleDecoderHINP()
: TModuleDecoder(kID,TRawDataTimingCharge::Class())
{
}

TModuleDecoderHINP::~TModuleDecoderHINP()
{
}

TModuleDecoderHINP::TModuleDecoderHINP(const TModuleDecoderHINP& rhs)
{
}

TModuleDecoderHINP& TModuleDecoderHINP::operator=(const TModuleDecoderHINP& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


Int_t TModuleDecoderHINP::Decode(char* buf, const Int_t &size, TObjArray *seg)
{
   UInt_t wcb = ((unsigned int*)buf)[0];
   UShort_t channelcount = ((unsigned short*)(buf + sizeof(int)/sizeof(char)))[0];
   ULong64_t timestamp = ((unsigned short*)(buf + sizeof(int)/sizeof(char) + sizeof(short)/sizeof(char)))[0];
   Int_t words = (size - 7 * sizeof(short))/sizeof(short); // subtract data size
   Short_t *bufs = (Short_t*)&buf[7*sizeof(short)];
   Int_t chip;
   Int_t ch;
   Int_t energy;
   Int_t time;
   for (Int_t i=0; i+2 <=  words; i+=3) {
      chip = ((bufs[i]>>5) & 0xff);
      ch   = (bufs[i] & 0xf);
      energy = (bufs[i+1]&0x3fff);
      time   = (bufs[i+2]&0x3fff);

      TRawDataTimingCharge *data = (TRawDataTimingCharge*) this->New();
      data->SetTiming(time);
      data->SetCharge(energy);
      data->SetSegInfo(seg->GetUniqueID(),chip,ch);
      seg->Add(data);
   }
   return 1;
}
