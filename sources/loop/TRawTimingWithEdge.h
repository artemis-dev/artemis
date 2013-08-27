/**
 * @file   TRawTimingWithEdge.h
 * @brief
 *
 * @date   Created:       2013-08-13 12:50:09
 *         Last Modified: 2013-08-14 08:56:01
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRAWTIMINGWITHEDGE_H
#define TRAWTIMINGWITHEDGE_H

#include <TRawTiming.h>

namespace art{

class TRawTimingWithEdge : public TRawTiming {
public:
   // Default constructor
   TRawTimingWithEdge();
   // Default destructor
   ~TRawTimingWithEdge();

   virtual Bool_t IsLeading() const {return fIsLeading;}
   void SetEdge(Bool_t edge) {fIsLeading = edge;}

   static const Bool_t kLeading = kTRUE;    //!
   static const Bool_t kTrailing = kFALSE;  //!

private:
   Bool_t fIsLeading;

   // Copy constructor (prohibited)
   TRawTimingWithEdge(const TRawTimingWithEdge& rhs);
   // Assignment operator (prohibited)
   TRawTimingWithEdge& operator=(const TRawTimingWithEdge& rhs);

   ClassDef(TRawTimingWithEdge,1);
};

}
#endif // TRAWTIMINGWITHEDGE_H
