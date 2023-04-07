/**
 * @file   TModuleDecoderTS.h
 * @brief  Decoder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:31:09
 *         Last Modified: 2013-07-25 16:15:17
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERVTS_H
#define TMODULEDECODERVTS_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderTS;
}

class art::TModuleDecoderTS : public TModuleDecoder{
  public:
   static const          int kID               = 3;
//   static const unsigned int kHeaderMask       = 0x06000000;
   static const unsigned int kHeaderMask       = 0xf000;
   static const unsigned int kTypeMask       = 0x00f0;
   static const unsigned int kIDMask       = 0x000f;
   //static const unsigned int kHeader           = 0x02000000;
   static const unsigned int kHeader           = 0x1;
   static const unsigned int kMeasure          = 0x0;
   static const unsigned int kEOB              = 0x04000000;
   //static const unsigned int kMaskGeometry     = 0xf8000000;
   static const unsigned int kMaskGeometry     = 0x0007;
   static const unsigned int kMaskChannel      = 0x7c00;
   static const unsigned int kMaskMeasure      = 0x03ff;
   static const          int kShiftGeometry    = 0;
   static const          int kShiftChannel     = 10;
   static const          int kShiftMeasure     = 0;

   // Default constructor
   TModuleDecoderTS();
   // Default destructor
   virtual ~TModuleDecoderTS();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

protected:
   TObjArray *fHitData;

private:
   // Copy constructor (prohibited)
   TModuleDecoderTS(const TModuleDecoderTS& rhs);
   // Assignment operator (prohibited)
   TModuleDecoderTS& operator=(const TModuleDecoderTS& rhs);

   ClassDef(TModuleDecoderTS,0);
};

#endif // TMODULEDECODERV7XX_H