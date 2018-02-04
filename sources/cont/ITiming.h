/**
 * @file   ITiming.h
 * @brief  interface for timing information
 *
 * @date   Created:       2013-11-15 12:59:01
 *         Last Modified: 2014-02-24 14:39:29
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef ITIMING_H
#define ITIMING_H

#include <Rtypes.h>

namespace art {
   class ITiming;
}

class art::ITiming {
public:
   virtual ~ITiming();
   virtual Double_t GetTiming() const = 0;
   virtual void SetTiming(Double_t) = 0;
   virtual void SetTiming(const ITiming&) = 0;

   virtual operator Double_t() const {return GetTiming();}
   virtual ITiming& operator=(Double_t val) {SetTiming(val); return *this;}
   virtual ITiming& operator+=(Double_t rhs) {return *this = *this + rhs;}
   virtual ITiming& operator-=(Double_t rhs) {return *this = *this - rhs;};

   ClassDef(ITiming,1) // interface for timing information
};

#endif // ITIMING_H
