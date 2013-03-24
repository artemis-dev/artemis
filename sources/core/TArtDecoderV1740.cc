/* $Id:$ */
/**
 * @file   TArtDecoderV1740.cc
 * @date   Created : Dec 24, 2011 19:24:19 JST
 *   Last Modified : Mar 06, 2013 17:37:24 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtDecoderV1740.h"

#include "TArtDecoderFactory.h"
#include "TArtFadcRawDataObject.h"

#include "TArtCore.h"
#include <stdio.h>
TArtDecoderV1740::TArtDecoderV1740()
   : TArtDecoder(kID) {
}

TArtDecoderV1740::~TArtDecoderV1740()
{
}

TArtDecoder* TArtDecoderV1740::Instance()
{
   static TArtDecoderV1740 instance;
   return &instance;
}

int TArtDecoderV1740::Decode(unsigned char* &buf, const unsigned int& size,
                             TArtRawSegmentObject* rawseg)
{
   unsigned int evtsize = size/sizeof(unsigned short);
   int i;
   int nw;
   unsigned int timestamp;
//   static int nSpill[6] = {0, 0, 0, 0, 0, 0};
//   if (!((evtdata & (1<<31)) && (evtdata & (1<<28)))) 

#ifndef V1740_RAW
#ifdef V1740_NON_SUP
   unsigned short* bufs = (unsigned short*) buf;
   TArtFadcRawDataObject *obj =
      new TArtFadcRawDataObject(2,0,0);
   for (i = 0; i < size/2; i++) {
      obj->AddSample(bufs[i]);
   }
   rawseg->PutData(obj);
#else // zero suppressed 
   // printf("zero suppressed\n");
   unsigned short* bufs = (unsigned short*) buf;
   
   for (UInt_t iw=0; iw < evtsize;) {
     unsigned int* bufi = (unsigned int*) bufs;
     if ((bufi[0]>>28) == 0xa) {
       // global header
       timestamp = bufi[2];
       nw = bufi[0]&0xffffff;
       Int_t geo = (bufi[1]>>27)&0x1f;
       if (((bufi[1]>>8)&0xffff) == 0xfff7) {
	  // printf("0x%08x\n",bufi[1]);
	 // spill on tag
	 TArtFadcRawDataObject *obj = 
            new TArtFadcRawDataObject(geo,-1,timestamp,0,bufi[0]);
	 //	  printf("spill tag %d at geo = %d timestamp = %u\n",nSpill[geo-1]++, geo,timestamp);
	  rawseg->PutData(obj);
//	 bufs += nw;
//	 iw   += nw;
//	 continue;
       }
       bufs += 6;
       iw += 6;
       nw -=6;
       
       for (Int_t iws = 0; iws < nw;) {
	 bufi = (unsigned int*) bufs;
	 if (((bufi[0])>>28) == 3) {
	   // sampling header
	   int geo = ((bufi[0]&0x0fff0000)>>16);
	   int ch  = (bufi[0]&0xff);
	   int nSamples = ((bufi[1]&0x3fff0000)>>16);
	   int pBegin = (bufi[1]&0xffff);
	   // printf("nSamples = %d\n",nSamples);
	   TArtFadcRawDataObject *obj = 
              new TArtFadcRawDataObject(geo,ch,timestamp,pBegin*2,0);
	   bufs += 4;
	   for (i = 0; i < nSamples; i++) {
	     obj->AddSample(bufs[i]);
	   }
	   rawseg->PutData(obj);
	   iws += 4 + nSamples;
	   iw += 4 + nSamples;
	   bufs += nSamples;
	 } else {
	   TArtCore::Info("Decode","Unknown Header 0x%08x",bufi[0]);
	   return 0;
	 }
       }
     }
   }
#endif
#else
   int ngr = 8;
   int nch = 8;
   int igr,nw,igeo;
   int ns;
   nw = ((evtdata[0]&0xfffffff)-4)>>3;
   igeo = ((evtdata[1])>>27);
   timestamp = evtdata[3];
   if ((evtdata[0]-4)&0x7) {
      TArtCore::Error("Decode","nsample is not multiple of 8\n");
      return;
   }
   evtdata += 4;
   for (igr=0; igr<ngr;igr++) {
      evtdata += 4 * nw;
      for (ich=0; ich<nch;ich++) {
         TArtFadcRawDataObject *obj = 
            new TArtFadcRawDataObject(igeo,ich+igr*nch,timestamp);
         for (iw = 0; iw < nw; iw+=9) {
            for (is=0; is<3; is++) {
               int bit = is * 12 + 36 * ich;
               int wd  = (bit>>5) + iw;
               int offset = (bit&0x1f);
               if (offset<20) {
                  adc = (evtdata[wd]>>offset)&0xfff;
               } else {
                  int off2 = 32 - offset;
                  int mask = (1<<(12-off2)) - 1;
                  adc = (evtdata[wd]>>offset)|
                     ((evtdata[wd+1]&mask)<<off2);
               }
               obj->AddSample(adc);
            }
         }
         rawseg->PutData(obj);
      }
   }
#endif
   return 0;
}
