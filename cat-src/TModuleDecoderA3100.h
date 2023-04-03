/**
 * @file   TModuleDecoderA3100.h
 * @brief  A3100 decoder
 *
 * @date   Created       : 2014-05-18 23:49:27 JST
 *         Last Modified :
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_5B7B7835_A164_4F4F_BF78_2A84FC9507A8
#define INCLUDE_GUARD_UUID_5B7B7835_A164_4F4F_BF78_2A84FC9507A8

#include "TModuleDecoder.h"

namespace art {
  class TModuleDecoderA3100;
}

class art::TModuleDecoderA3100 : public TModuleDecoder {
 public:
  static const int kID = 31;
  TModuleDecoderA3100();
  virtual ~TModuleDecoderA3100();
  virtual Int_t Decode(char* buffer, const Int_t &size,
		       TObjArray *seg);

  //  TModuleDecoderA3100(const TModuleDecoderA3100& rhs);
  //  TModuleDecoderA3100& operator=(const TModuleDecoderA3100& rhs);

 protected:
  TObjArray *fHitData; // array to tomprally store the data for the aggregation

  ClassDef(TModuleDecoderA3100,0) // A3100 decoder
};
#endif // INCLUDE_GUARD_UUID_5B7B7835_A164_4F4F_BF78_2A84FC9507A8
