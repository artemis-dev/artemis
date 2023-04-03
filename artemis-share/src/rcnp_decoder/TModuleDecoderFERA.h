/**
 * @file   TModuleDecoderFERA.h
 * @brief  Decoder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:31:09
 *         Last Modified: 2013-07-25 16:15:17
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERVFERA_H
#define TMODULEDECODERVFERA_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderFERA;
}

class art::TModuleDecoderFERA : public TModuleDecoder{
  public:
//   static const          int kID               = 1;//2016506 fixed
   static const          int kID               = 1;//2016506 fixed
//   static const unsigned int kHeaderMask       = 0x06000000;
   static const unsigned int kHeaderMask       = 0x8000;
   //static const unsigned int kHeader           = 0x02000000;
   static const unsigned int kHeader           = 0x8000;
   static const unsigned int kMeasure          = 0x0000;
   static const unsigned int kEOB              = 0x04000000;
   //static const unsigned int kMaskGeometry     = 0xf8000000;
//   static const unsigned int kMaskGeometry     = 0x0007;
   static const unsigned int kMaskGeometry     = 0x00ff;
   static const unsigned int kMaskChannel      = 0x7800;
   static const unsigned int kMaskMeasure      = 0x07ff;
   static const          int kShiftGeometry    = 0;
   static const          int kShiftChannel     = 11;
   static const          int kShiftMeasure     = 0;

   // Default constructor
   TModuleDecoderFERA();
   // Default destructor
   virtual ~TModuleDecoderFERA();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

protected:
   TObjArray *fHitData;

private:
   // Copy constructor (prohibited)
   TModuleDecoderFERA(const TModuleDecoderFERA& rhs);
   // Assignment operator (prohibited)
   TModuleDecoderFERA& operator=(const TModuleDecoderFERA& rhs);

   ClassDef(TModuleDecoderFERA,0);
};

#endif // TMODULEDECODERV7XX_H
