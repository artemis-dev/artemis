/* @file TResponseFunctionNArray.h
 * @brief responsee function TNArray
 *
 * @date Create        : 2020-05-27 14:15:09 JST
 *       Last Modified : 2020-05-27 14:29:01 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_B8ACDAF8_1F4A_4C0C_A0B7_E871576EE16B
#define INCLUDE_GUARD_UUID_B8ACDAF8_1F4A_4C0C_A0B7_E871576EE16B

#include "TNArray.h"

namespace art {
   class TResponseFunctionNArray;
}


class art::TResponseFunctionNArray : public TNArray {
public:
   TResponseFunctionNArray();
   virtual ~TResponseFunctionNArray();

   virtual double Eval(double *x) const {
      x[1] = EncodeDistance(x[1]);
      return TNArray::Eval2(x);
   }
   
   virtual double DecodeDistance(double r) const { return TMath::Tan(r)/0.4 + 3; }
   virtual double EncodeDistance(double d) const { return TMath::ATan(0.4*(d - 3.)); }

   ClassDef(TResponseFunctionNArray,1)
}; // end of TResponseFunctionNArray

#endif // #ifndef INCLUDE_GUARD_UUID_B8ACDAF8_1F4A_4C0C_A0B7_E871576EE16B}
