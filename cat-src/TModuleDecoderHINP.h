/**
 * @file   TModuleDecoderHINP.h
 * @brief  module decoder for hinp
 *
 * @date   Created       : 2015-10-04 16:51:20 JST
 *         Last Modified : 2015-10-04 17:20:57 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_F5449ED7_C28F_42CB_A4BA_3296EBF45E7D
#define INCLUDE_GUARD_UUID_F5449ED7_C28F_42CB_A4BA_3296EBF45E7D

#include "TModuleDecoder.h"

namespace art {
   class TModuleDecoderHINP;
}

class art::TModuleDecoderHINP : public TModuleDecoder {
public:
  static const int kID = 53;   
   TModuleDecoderHINP();
   virtual ~TModuleDecoderHINP();

   TModuleDecoderHINP(const TModuleDecoderHINP& rhs);
   TModuleDecoderHINP& operator=(const TModuleDecoderHINP& rhs);

   virtual Int_t Decode(char* buffer, const Int_t &size,TObjArray *seg);

protected:

private:

   ClassDef(TModuleDecoderHINP,1) // module decoder for hinp
};

#endif // INCLUDE_GUARD_UUID_F5449ED7_C28F_42CB_A4BA_3296EBF45E7D
