/**
 * @file   TModuleDecoderV550.h
 * @brief  V550 decoder
 *
 * @date   Created       : 2014-05-18 23:49:27 JST
 *         Last Modified :
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef TMODULEDECODERV550_H
#define TMODULEDECODERV550_H

#include "TModuleDecoder.h"

namespace art {
  class TModuleDecoderV550;
}

class art::TModuleDecoderV550 : public TModuleDecoder {
 public:
  static const int kID = 22;
  static const int kDCNUnit = 32;
  TModuleDecoderV550();
  virtual ~TModuleDecoderV550();
  virtual Int_t Decode(char* buffer, const Int_t &size,
		       TObjArray *seg);

  //  TModuleDecoderV550(const TModuleDecoderV550& rhs);
  //  TModuleDecoderV550& operator=(const TModuleDecoderV550& rhs);

 protected:
  TObjArray *fHitData; // array to tomprally store the data for the aggregation

  static const int kGeoMask       = 0xf8000000;
  static const int kFifoIdMask    = 0x10000000;
  static const int kNDataMask     = 0x000007ff;
  static const int kNChannelMask  = 0x003f0000;
  static const int kValidityMask  = 0x40000000;
  static const int kOverrangeMask = 0x80000000;
  static const int kChannelMask   = 0x007ff000;
  static const int kAdcMask       = 0x00000fff;

  static const int kGeoShift       = 27;
  static const int kNDataShift     = 0;
  static const int kNChannelShift  = 16;
  static const int kValidityShift  = 30;
  static const int kOverrangeShift = 31;
  static const int kFifoIdShift    = 28;
  static const int kChannelShift   = 12;
  static const int kAdcShift       = 0;

  ClassDef(TModuleDecoderV550,0) // V550 decoder
};
#endif // TMODULEDECODERV550_H
