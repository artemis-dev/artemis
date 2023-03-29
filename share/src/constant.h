/**
 * @file   constant.h
 * @brief
 *
 * @date   Created:       2013-10-10 13:14:47
 *         Last Modified: 2014-02-24 14:53:45
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef CONSTANT_H
#define CONSTANT_H

#include <TMath.h>

namespace art{

   const Int_t    kInvalidI = -1e+8;
   const Double_t kInvalidD = TMath::QuietNaN();

   inline Bool_t IsValid(Int_t val)    { return val != kInvalidI; }
//   Bool_t IsValid(Double_t val) { return val != kInvalidD; } // use if kInvalidD is NOT nan
   inline Bool_t IsValid(Double_t val) { return TMath::Finite(val); } // use if kInvalidD is nan

   const Double_t P_MASS = 938.272;
   const Double_t N_MASS = 939.565;
}

#endif // CONSTANT_H
