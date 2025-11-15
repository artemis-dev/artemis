/**
 * @file   TModuleDecoderA3100TriggeredList.h
 * @brief  A3100 decoder for Triggered List Mode
 *
 * @author Rin Yokoyama <yokoyama@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Rin Yokoyama	
 */

#ifndef TMODULEDECODERA3100TRIGGEREDLIST_H
#define TMODULEDECODERA3100TRIGGEREDLIST_H

#include "TModuleDecoder.h"

namespace art {
  class TModuleDecoderA3100TriggeredList;
}

class art::TModuleDecoderA3100TriggeredList : public TModuleDecoder {
 public:
  static const int kID = 130;
  TModuleDecoderA3100TriggeredList();
  virtual ~TModuleDecoderA3100TriggeredList();
  virtual Int_t Decode(char* buffer, const Int_t &size,
		       TObjArray *seg);

 protected:
  TObjArray *fHitData; // array to tomprally store the data for the aggregation

  ClassDef(TModuleDecoderA3100TriggeredList,0) // A3100 decoder

  static const int kHeaderMask = 0xE0000000;
  static const int kFirstWord = 0xC0000000;
  static const int kSecondWord = 0xE0000000;
  static const int kThiredWord = 0x60000000;

  static const int kADCMask = 0x00001FFF;
  static const int kChMask = 0x0003C000;
  static const int kChShift = 14;
  static const int kTSI_HiMask = 0x1FFC0000;
  static const int kTSI_HiShift = 18;
  static const int kTSI_LoMask = 0x1FFFFFFF;
  static const int kEventCountMask = 0x0FFFFFFF;
};
#endif // INCLUDE_GUARD_UUID_5B7B7835_A164_4F4F_BF78_2A84FC9507A8
