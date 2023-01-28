/**
 * @file   TRawTimingWithEdge.h
 * @brief
 *
 * @date   Created:       2013-08-13 12:50:09
 *         Last Modified: 2013-11-12 15:26:49
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRAWTIMINGWITHEDGE_H
#define TRAWTIMINGWITHEDGE_H

#include "TRawTiming.h"

namespace art{

class TRawTimingWithEdge : public TRawTiming {
public:
   // Default constructor
   TRawTimingWithEdge();
   virtual ~TRawTimingWithEdge();

   virtual Bool_t IsLeading() const {return fIsLeading;}
   void SetEdge(Bool_t edge) {fIsLeading = edge;}

   static const Bool_t kLeading = kTRUE;    //!
   static const Bool_t kTrailing = kFALSE;  //!

   virtual void Clear(Option_t *opt="") {
      TRawTiming::Clear(opt);
      fIsLeading = kTRUE;
   }

private:
   Bool_t fIsLeading;

   // Copy constructor (prohibited)
   TRawTimingWithEdge(const TRawTimingWithEdge&);
   // Assignment operator (prohibited)
   TRawTimingWithEdge& operator=(const TRawTimingWithEdge&);

   ClassDef(TRawTimingWithEdge,1);
};

}
#endif // TRAWTIMINGWITHEDGE_H
