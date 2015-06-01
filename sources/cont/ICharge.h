/**
 * @file   ICharge.h
 * @brief
 *
 * @date   Created:       2013-11-15 13:10:35
 *         Last Modified: 2014-02-24 14:38:58
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef ICHARGE_H
#define ICHARGE_H

#include <Rtypes.h>

namespace art {
   class ICharge;
}

class art::ICharge {
public:
   virtual ~ICharge();
   virtual Double_t GetCharge() const = 0;
   virtual void SetCharge(Double_t) = 0;
   virtual void SetCharge(const ICharge&) = 0;

   virtual operator Double_t() const {return GetCharge();}
   virtual ICharge& operator=(Double_t val) {SetCharge(val); return *this;}
   virtual ICharge& operator+=(Double_t rhs) {return *this = *this + rhs;}
   virtual ICharge& operator-=(Double_t rhs) {return *this = *this - rhs;};

   ClassDef(ICharge,1) // interface for charge information
};

#endif // ICHARGE_H
