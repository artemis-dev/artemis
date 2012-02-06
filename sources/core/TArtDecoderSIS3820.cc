/* $Id:$ */
/**
 * @file   TArtDecoderSIS3820.cc
 * @date   Created : Dec 24, 2011 19:24:19 JST
 *   Last Modified : Jan 13, 2012 02:07:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtDecoderSIS3820.h"

#include "TArtDecoderFactory.h"
#include "TArtFadcRawDataObject.h"

#include "TArtCore.h"
#include <stdio.h>
TArtDecoderSIS3820::TArtDecoderSIS3820()
   : TArtDecoder(kID) {
}

TArtDecoderSIS3820::~TArtDecoderSIS3820()
{
}

TArtDecoder* TArtDecoderSIS3820::Instance()
{
   static TArtDecoderSIS3820 instance;
   return &instance;
}

int TArtDecoderSIS3820::Decode(unsigned char* &buf, const unsigned int& size,
                             TArtRawSegmentObject* rawseg)
{

   for (Int_t i=0; i<32; i++) {
      rawseg->PutData(new TArtRawDataObject(0,i,(((int*)buf)[i])));
   }
   return 0;
}
