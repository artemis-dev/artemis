/* $Id:$ */
/**
 * @file   TModuleDecoderV1740_mod.cc
 * @date   Created : Feb 06, 2013 15:06:29 JST
 *   Last Modified : 2015-04-27 10:52:07 JST by tokieda
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TModuleDecoderV1740_mod.h"
#include <TRawDataFadc.h>

art::TModuleDecoderV1740_mod::TModuleDecoderV1740_mod()
   : TModuleDecoder(kID,TRawDataFadc::Class())
{
}
art::TModuleDecoderV1740_mod::~TModuleDecoderV1740_mod()
{
}

Int_t art::TModuleDecoderV1740_mod::Decode(char* buf, const int &size, TObjArray *seg)
{
   unsigned int evtsize = size/sizeof(unsigned short);
   int i;
   int nw;
   int geo, pattern;
   unsigned int timestamp;
   int post, ch, begin, end;
   int total;
   unsigned short* bufs = (unsigned short*) buf;
   
   for (UInt_t iw=0; iw < evtsize;) {
      unsigned int* bufi = (unsigned int*) bufs;
      if (bufi[0] ==0) return 0;
      if (bufi[0]&0xd0000000) {
         // global header
         timestamp = bufi[2];
         nw        = (bufi[0]&0xfffffff);
         geo       = ((bufi[1]>>27)&0x1f);
         pattern   = ((bufi[1]>>8)&0xffff);
         total     = ((bufi[3]>>16)&0xffff);
         post      = ((bufi[3])&0xffff);

         bufs+=8;
         iw  +=8;
         nw  -=8;
         if (nw==0) {
            //          if (((bufi[1]>>8)&0xffff) == 0xfffe) {
            TRawDataFadc* data = (TRawDataFadc*) New();
            Int_t ch = 64;
            Int_t len = 0;
            data->Clear("C");
            data->SetSegInfo(seg->GetUniqueID(),geo,ch);
            data->SetFadcInfo(timestamp,len,pattern);
            seg->Add(data);
            continue;
         }
         for (Int_t iws = 0; iws < nw;) {
            bufi = (unsigned int*) bufs;
            if (((bufi[0])>>28) == 3) {
               // sampling header
               int geo = ((bufi[0]&0x0fff0000)>>16);
               int ch  = (bufi[0]&0xff);
               int nSamples = ((bufi[1]&0x3fff0000)>>16);
               int pBegin = (bufi[1]&0xffff);
               TRawDataFadc *data = (TRawDataFadc*) New();
               data->Clear("C");
               data->SetSegInfo(seg->GetUniqueID(),geo,ch);
               data->SetFadcInfo(timestamp,pBegin,pattern);
               bufs += 4;
               for (i = 0; i < nSamples; i++) {
                  data->Add((Int_t)bufs[i]);
               }
               seg->Add(data);
               iws += 4 + nSamples;
               iw += 4 + nSamples;
               bufs += nSamples;
            } else if ((bufi[0]&0xf0000000)==(0x50000000)){
               // int hit  = ((bufi[0]>>8)&0xff);
               ch   = ((bufi[0])&0xff);
               begin = ((bufi[1]>>16)&0xffff);
               end   = (bufi[1]&0xffff);
               int sample = end - begin;
               TRawDataFadc *data = (TRawDataFadc*) New();
               seg->Add(data);
               data->Clear("C");
               data->SetSegInfo(seg->GetUniqueID(),geo,ch);
               data->SetFadcInfo(timestamp,(post - total + begin),pattern);
               bufs+=4;
               for (i = 0; i!=sample; i++) {
                  data->Add(bufs[i]);
               }
               iws += (4 + sample);
               iw  += (4 + sample);
               bufs += sample;
            } else {
               //if (fVerboseLevel>0) Warning("Decode","Unknown Header 0x%08x",bufi[0]);
               return 0;
            }
         }
      }
   }
   return 0;
}
