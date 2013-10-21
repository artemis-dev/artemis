/**
 * @file   TRawTiming.h
 * @brief
 *
 * @date   Created:       2013-08-13 12:18:30
 *         Last Modified: 2013-10-21 10:21:11
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

   virtual Int_t GetTiming() const {return (Int_t)Get() - fReference;}
   virtual void SetTiming(const UInt_t val) {Set(val);}
   virtual Bool_t IsLeading() const {return kTRUE;}

   virtual UInt_t GetReference() {return fReference;}
   virtual void SetReference(const UInt_t val) {fReference = val;}

   virtual void Clear(Option_t *opt="") {
      TRawDataSimple<UInt_t>::Clear(opt);
      fReference = 0;
   }

protected:
   UInt_t fReference;

private:
   // Copy constructor (prohibited)
   TRawTiming(const TRawTiming& rhs);
   // Assignment operator (prohibited)
   TRawTiming& operator=(const TRawTiming& rhs);

   ClassDef(TRawTiming,1);

};

}
#endif // TRAWTIMING_H


