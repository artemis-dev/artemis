/* $Id:$ */
/**
 * @file   TModuleDecoderSIS3610.h
 * @date   Created : Dec 24, 2011 19:24:28 JST
 *   Last Modified : Oct 31, 2013 06:06:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTDECODERSIS3610_H
#define TARTDECODERSIS3610_H

#include "TModuleDecoder.h"

namespace art {
   class TModuleDecoderSIS3610;
}

class art::TModuleDecoderSIS3610  : public art::TModuleDecoder {
public:   
   static const int kID = 37;

   TModuleDecoderSIS3610();
   virtual ~TModuleDecoderSIS3610();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

   ClassDef(TModuleDecoderSIS3610,1)
};
#endif // end of #ifdef TARTDECODERSIS3610_H
