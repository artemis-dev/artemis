/**
 * @file   T1DGatingConverter.h
 * @brief  converter for 1D gating
 *
 * @date   Created       : 2015-03-16 10:16:27 JST
 *         Last Modified : 2015-03-16 11:02:31 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_C930DF0D_C56F_490B_BFD3_B721AD54460A
#define INCLUDE_GUARD_UUID_C930DF0D_C56F_490B_BFD3_B721AD54460A

/////////////////////////////////////////////////////////
//                                                     //
// art::T1DGatingCoverter                              //
//                                                     //
// 1D gate converter returns kInvalidD                 //
// if val < min or val > max.                          //
//                                                     //
// initial value is (min, max) = (-inf,inf).           //
// you can use either of infinity and nan              //
// by "-inf", "inf", or "nan" in your parameter files. //
//                                                     //
/////////////////////////////////////////////////////////

#include "TFixedNumberParameter.h"

namespace art {
   class T1DGatingConverter;
}

class art::T1DGatingConverter : public TFixedNumberParameter {
public:
   T1DGatingConverter();
   T1DGatingConverter(Double_t min, Double_t max);
   virtual ~T1DGatingConverter();

   virtual void Set(Double_t min, Double_t max);
   virtual Double_t Convert(Double_t val) const;
   virtual void Print(Option_t* opt = "") const;

   ClassDef(T1DGatingConverter,1) // converter for 1D gating
};

#endif // INCLUDE_GUARD_UUID_C930DF0D_C56F_490B_BFD3_B721AD54460A
