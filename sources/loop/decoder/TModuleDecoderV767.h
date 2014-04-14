/**
 * @file   TModuleDecoderV767.h
 * @brief  Decoder Class for V767
 *
 * @date   Created:       2013-07-23 10:28:45
 *         Last Modified: 2013-07-25 16:14:18
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERV767_H
#define TMODULEDECODERV767_H
#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV767;
}

class art::TModuleDecoderV767 : public TModuleDecoder {
public:
   static const          int kID               = 23;
   static const unsigned int kHeaderMask       = 0x00600000;
   static const unsigned int kHeader           = 0x00400000;
   static const unsigned int kMeasure          = 0x00000000;
   static const unsigned int kEOB              = 0x00200000;
   static const unsigned int kError            = 0x00600000;
   static const unsigned int kMaskGeometry     = 0xf8000000;
   static const unsigned int kMaskEventNumber  = 0x00000fff;
   static const unsigned int kMaskChannel      = 0x7f000000;
   static const unsigned int kMaskMeasure      = 0x000fffff;
   static const unsigned int kMaskEdgeType     = 0x00100000;
   static const          int kShiftGeometry    = 27;
   static const          int kShiftEventNumber = 0;
   static const          int kShiftChannel     = 24;
   static const          int kShiftMeasure     = 0;
   static const          int kShiftEdgeType    = 20;

   // Default constructor
   TModuleDecoderV767();
   // Default destructor
   virtual ~TModuleDecoderV767();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

protected:
   TObjArray *fHitData;

private:
   // Copy constructor (prohibited)
   TModuleDecoderV767(const TModuleDecoderV767& rhs);
   // Assignment operator (prohibited)
   TModuleDecoderV767& operator=(const TModuleDecoderV767& rhs);


   ClassDef(TModuleDecoderV767,0);
};

#endif // TMODULEDECODERV767_H
