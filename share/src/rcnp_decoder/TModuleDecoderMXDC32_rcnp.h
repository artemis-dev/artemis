/**
 * @file   TModuleDecoderMXDC32_rcnp.h
 * @brief  Decoder Class for MADC32, MQDC32, MTDC32
 *         This code was adopted from AnaROOT
 *
 * @date   Created       : 2015-10-08 11:26:12 JST
 *         Last Modified : 2015-10-08 15:46:13 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef TMODULEDECODERMXDC32_RCNP_H
#define TMODULEDECODERMXDC32_RCNP_H

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderMXDC32_rcnp;
}

class art::TModuleDecoderMXDC32_rcnp : public TModuleDecoder {
public:
   TModuleDecoderMXDC32_rcnp();
   explicit TModuleDecoderMXDC32_rcnp(Int_t id);
   virtual ~TModuleDecoderMXDC32_rcnp();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

   static const UInt_t kID            = 32;
   static const UInt_t kChannel       = 32;
   static const UInt_t kHeader        = 0x01;
   static const UInt_t kMeasure       = 0x00;
   //static const UInt_t kEOB           = 0x11;
   static const UInt_t kEOB           = 0x03; //Y. HIJIKATA
   static const UInt_t kMaskHeader    = 0xc0000000;
   static const UInt_t kMaskGeometry  = 0x00ff0000;
   static const UInt_t kMaskChannel   = 0x001f0000;
   static const UInt_t kMaskMeasure   = 0x0000ffff;
   static const  Int_t kShiftHeader   = 30;
   static const  Int_t kShiftGeometry = 16;
   static const  Int_t kShiftChannel  = 16;
   static const  Int_t kShiftMeasure  = 0;

protected:
   TObjArray *fHitData; // array to temporally store the data for the aggregation

private:
   TModuleDecoderMXDC32_rcnp(const TModuleDecoderMXDC32_rcnp&); // not defined
   TModuleDecoderMXDC32_rcnp& operator=(const TModuleDecoderMXDC32_rcnp&); // not defined

   ClassDef(TModuleDecoderMXDC32_rcnp,0) // Decoder Class for MADC32, MQDC32, MTDC32
};

#endif // TMODULEDECODERMXDC32_RCNP_H
