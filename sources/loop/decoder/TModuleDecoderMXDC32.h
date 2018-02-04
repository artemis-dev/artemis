/**
 * @file   TModuleDecoderMXDC32.h
 * @brief  Decoder Class for MADC32, MQDC32, MTDC32
 *         This code was adopted from AnaROOT
 *
 * @date   Created       : 2015-10-08 11:26:12 JST
 *         Last Modified : 2015-10-08 15:46:13 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_2663303E_075F_49B3_89A8_CEC7F8A7DAA7
#define INCLUDE_GUARD_UUID_2663303E_075F_49B3_89A8_CEC7F8A7DAA7

#include <TModuleDecoder.h>

namespace art {
   class TModuleDecoderMXDC32;
}

class art::TModuleDecoderMXDC32 : public TModuleDecoder {
public:
   TModuleDecoderMXDC32();
   explicit TModuleDecoderMXDC32(Int_t id);
   virtual ~TModuleDecoderMXDC32();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);

   static const UInt_t kID            = 32;
   static const UInt_t kChannel       = 32;
   static const UInt_t kHeader        = 0x01;
   static const UInt_t kMeasure       = 0x00;
   static const UInt_t kEOB           = 0x11;
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
   TModuleDecoderMXDC32(const TModuleDecoderMXDC32&); // not defined
   TModuleDecoderMXDC32& operator=(const TModuleDecoderMXDC32&); // not defined

   ClassDef(TModuleDecoderMXDC32,0) // Decoder Class for MADC32, MQDC32, MTDC32
};

#endif // INCLUDE_GUARD_UUID_2663303E_075F_49B3_89A8_CEC7F8A7DAA7
