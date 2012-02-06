/* $Id:$ */
/**
 * @file   TArtDecoderSIS3610.h
 * @date   Created : Dec 24, 2011 19:24:28 JST
 *   Last Modified : Jan 31, 2012 14:41:09 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTDECODERSIS3610_H
#define TARTDECODERSIS3610_H

#include "TArtDecoder.h"
class TArtDecoderSIS3610  : public TArtDecoder {
private:
   TArtDecoderSIS3610();
   TArtDecoderSIS3610(const TArtDecoderSIS3610&/*rhs*/) 
      : TArtDecoder(kID) {;}
public:
   static const int kID = 37;
   static TArtDecoder* Instance();
   ~TArtDecoderSIS3610();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

};
#endif // end of #ifdef TARTDECODERSIS3610_H
