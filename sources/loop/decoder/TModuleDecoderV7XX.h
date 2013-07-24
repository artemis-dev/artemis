/**
 * @file   TModuleDecoderV7XX.h
 * @brief  Decoder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:31:09
 *         Last Modified: 2013-07-24 14:53:34
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERV7XX_H
#define TMODULEDECODERV7XX_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV7XX;
}

class art::TModuleDecoderV7XX : public TModuleDecoder{
  public:
   static const          int kID               = 21;
   static const unsigned int kHeaderMask       = 0x06000000;
   static const unsigned int kHeader           = 0x02000000;
   static const unsigned int kMeasure          = 0x00000000;
   static const unsigned int kEOB              = 0x04000000;
   static const unsigned int kMaskGeometry     = 0xf8000000;
   static const unsigned int kMaskChannel      = 0x003f0000;
   static const unsigned int kMaskMeasure      = 0x00001fff;
   static const          int kShiftGeometry    = 27;
   static const          int kShiftChannel     = 16;
   static const          int kShiftMeasure     = 0;

   // Default constructor
   TModuleDecoderV7XX();
   // Default destructor
   virtual ~TModuleDecoderV7XX();
   // Copy constructor
   TModuleDecoderV7XX(const TModuleDecoderV7XX& rhs);
   // Assignment operator
   TModuleDecoderV7XX& operator=(const TModuleDecoderV7XX& rhs);

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

protected:
   TObjArray *fHitData;
};

#endif // TMODULEDECODERV7XX_H
