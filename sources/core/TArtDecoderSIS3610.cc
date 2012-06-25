/* $Id:$ */
/**
 * @file   TArtDecoderSIS3610.cc
 * @date   Created : Dec 24, 2011 19:24:19 JST
 *   Last Modified : Jan 13, 2012 02:07:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtDecoderSIS3610.h"

#include "TArtDecoderFactory.h"
#include "TArtFadcRawDataObject.h"

#include "TArtCore.h"
#include <stdio.h>
TArtDecoderSIS3610::TArtDecoderSIS3610()
   : TArtDecoder(kID) {
}

TArtDecoderSIS3610::~TArtDecoderSIS3610()
{
}

TArtDecoder* TArtDecoderSIS3610::Instance()
{
   static TArtDecoderSIS3610 instance;
   return &instance;
}

int TArtDecoderSIS3610::Decode(unsigned char* &buf, const unsigned int& size,
                             TArtRawSegmentObject* rawseg)
{

   rawseg->PutData(new ((*fHits)[fHits->GetEntriesFast()]) TArtRawDataObject(0,0,(int)*((short*)buf)));
   return 0;
}
