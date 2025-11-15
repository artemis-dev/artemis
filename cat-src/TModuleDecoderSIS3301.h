/*
 * @file TModuleDecoderSIS3301.h
 * @date  Created : 
 *  Last Modified : Jul 15, 2014 03:18:03 JST
 *--------------------------------------------------------
 *    Comment : 
 *
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#ifndef TMODULEDECODERSIS3301_H
#define TMODULEDECODERSIS3301_H
#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderSIS3301;
}

class art::TModuleDecoderSIS3301  : public TModuleDecoder {
public:
   static const int kID = 8;

   TModuleDecoderSIS3301();         // constructor with default id = kID for compatibility
   virtual ~TModuleDecoderSIS3301();
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

   static const unsigned int kHeaderMask        = 0xf0000000;
   static const unsigned int kHeaderMask2       = 0xff000000;
   static const unsigned int kADCEvtHeader      = 0xc0000000;
   static const unsigned int kADCTIMEHeader     = 0x8a000000;
   static const unsigned int kMaskGeometry      = 0x1fff0000;
   static const unsigned int kMaskChannel       = 0x000000ff;
   static const unsigned int kMaskPagesize      = 0x1f000000;
   static const unsigned int kMaskEventID       = 0x0000ff00;
   static const unsigned int kMaskClock         = 0x00ffffff;
   static const unsigned int kMaskData1st       = 0x0000ffff;
   static const unsigned int kMaskData2nd       = 0xffff0000;
   static const int kShiftGeometry     = 16;
   static const int kShiftChannel      =  0;
   static const int kShiftPagesize     = 24;
   static const int kShiftEventID      =  8;
   static const int kShiftClock        =  0;
   static const int kShiftData1st      =  0;
   static const int kShiftData2nd      = 16;

protected:

   ClassDef(TModuleDecoderSIS3301,0); // decoder for module SIS3301
};
#endif // end of #ifdef TMODULEDECODERSIS3301_H
