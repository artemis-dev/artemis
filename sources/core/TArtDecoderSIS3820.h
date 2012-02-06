/* $Id:$ */
/**
 * @file   TArtDecoderSIS3820.h
 * @date   Created : Dec 24, 2011 19:24:28 JST
 *   Last Modified : Jan 31, 2012 14:41:23 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTDECODERSIS3820_H
#define TARTDECODERSIS3820_H

#include "TArtDecoder.h"
class TArtDecoderSIS3820  : public TArtDecoder {
private:
   TArtDecoderSIS3820();
   TArtDecoderSIS3820(const TArtDecoderSIS3820& /* rhs */) 
      : TArtDecoder(kID) {;}
public:
   //   static const int kID = 1; // temporal
   static const int kID = 36;
   static TArtDecoder* Instance();
   ~TArtDecoderSIS3820();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

};
#endif // end of #ifdef TARTDECODERSIS3820_H
