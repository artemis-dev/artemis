/*
 * @file TModuleDecoderV1190_rcnp.h
 * @date  Created : 2008/11/26 21:30:29 JST
 *  Last Modified : 2014-03-11 12:48:43 JST (kawase)
 *--------------------------------------------------------
 *    Comment : modified to use data container TRawTimingWithEdge
 *
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#ifndef TMODULEDECODERV1190_RCNP_H
#define TMODULEDECODERV1190_RCNP_H
#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderV1190_rcnp;
}

class art::TModuleDecoderV1190_rcnp  : public TModuleDecoder {
public:
   static const int kID = 24;

   TModuleDecoderV1190_rcnp();         // constructor with default id = kID for compatibility
   TModuleDecoderV1190_rcnp(Int_t id); // constructor with id
   virtual ~TModuleDecoderV1190_rcnp();
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

   static const unsigned int kHeaderMask        = 0xf8000000;
   static const unsigned int kGlobalHeader      = 0x40000000;
   static const unsigned int kTDCHeader         = 0x08000000;
   static const unsigned int kTDCMeasurement    = 0x00000000;
   static const unsigned int kTDCTrailer        = 0x18000000;
   static const unsigned int kTDCError          = 0x20000000;
   static const unsigned int kGlobalTrailer     = 0x80000000;
   static const unsigned int kMaskGeometry      = 0x0000001f;
   static const unsigned int kMaskEventCounter  = 0x7ffffe0;
   static const unsigned int kMaskBunchID       = 0x00000fff;
   static const unsigned int kMaskEventID       = 0x00000fff;
   static const unsigned int kMaskChannel       = 0x03f80000;
   static const unsigned int kMaskMeasure       = 0x0007ffff;
   static const unsigned int kMaskEdgeType      = 0x04000000;
   static const int kShiftGeometry     = 0;
   static const int kShiftEventCounter = 5;
   static const int kShiftBunchID      = 0;
   static const int kShiftEventID      = 12;
   static const int kShiftChannel      = 19;
   static const int kShiftMeasure      = 0;
   static const int kShiftEdgeType     = 26;

protected:
   TObjArray *fHitData; // array to temporally store the data for the aggregation

   ClassDef(TModuleDecoderV1190_rcnp,0); // decoder for module V1190
};
#endif // end of #ifdef TMODULEDECODERV1190_RCNP_H
