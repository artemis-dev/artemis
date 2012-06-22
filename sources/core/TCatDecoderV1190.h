/* $Id:$ */
/**
 * @file   TCatDecoderV1190.h
 * @date   Created : Jun 22, 2012 19:22:24 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATDECODERV1190_H
#define TCATDECODERV1190_H

#include <TArtRawSegmentObject.h>
#include <TCatRawDataV1190.h>
#include <TClonesArray.h>
#include <TArtDecoder.h>

class TCatDecoderV1190 : public TArtDecoder{

public:
   TCatDecoderV1190();
   virtual ~TCatDecoderV1190();

   virtual Int_t Decode(unsigned char *&buf, const unsigned int& size,
                        TArtRawSegmentObject *rawseg);
   
   void TrailingIsFirst(Int_t ch) { fLeadingIsFirst[ch] = kFALSE; }
   virtual void ClearData() { fV1190Hits->Clear(); }

public:
   static const Int_t kID = 24;
   static const Int_t kMaxCh = 128;
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
   TClonesArray *fV1190Hits;
   TCatRawDataV1190 *fIdx[kMaxCh];
   Bool_t            fLeadingIsFirst[kMaxCh]; // default leading is first

};
#endif // end of #ifdef TCATDECODERV1190_H
