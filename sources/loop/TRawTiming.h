/**
 * @file   TRawTiming.h
 * @brief
 *
 * @date   Created:       2013-08-13 12:18:30
 *         Last Modified: 2013-08-15 11:54:19
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRAWTIMING_H
#define TRAWTIMING_H

#include <TRawDataSimple.h>

namespace art{

class TRawTiming : public TRawDataSimple<UInt_t> {
public:
   // Default constructor
   TRawTiming();
   // Default destructor
   ~TRawTiming();

   virtual UInt_t GetTiming() {return Get();}
   virtual void SetTiming(UInt_t val) {Set(val);}
   virtual Bool_t IsLeading() const {return kTRUE;}

private:
   // Copy constructor (prohibited)
   TRawTiming(const TRawTiming& rhs);
   // Assignment operator (prohibited)
   TRawTiming& operator=(const TRawTiming& rhs);

   ClassDef(TRawTiming,1);

};

}
#endif // TRAWTIMING_H


