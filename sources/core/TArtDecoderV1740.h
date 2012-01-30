/* $Id:$ */
/**
 * @file   TArtDecoderV1740.h
 * @date   Created : Dec 24, 2011 19:24:28 JST
 *   Last Modified : Jan 13, 2012 02:11:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTDECODERV1740_H
#define TARTDECODERV1740_H

#include "TArtDecoder.h"
class TArtDecoderV1740  : public TArtDecoder {
private:
   TArtDecoderV1740();
   TArtDecoderV1740(const TArtDecoderV1740&rhs) {;}
public:
   //   static const int kID = 1; // temporal
   static const int kID = 63;
   static TArtDecoder* Instance();
   ~TArtDecoderV1740();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);

#if 0
   const int kHeaderBit = (1<<31) | (1<<29);
   const int kHeaderMask = (0xf << 28);
   
   const int kBoadIdShift = 27;
   const int kBoadIdMask  = 0x1f;
#endif
};
#endif // end of #ifdef TARTDECODERV1740_H
