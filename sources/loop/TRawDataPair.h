/**
 * @file   TRawDataPair.h
 * @brief  Rawdata container with two values
 *
 * @date   Created:       2013-07-25 10:19:08
 *         Last Modified: 2013-07-25 12:03:18
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 * @note   This class contains two Int_t values
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRAWDATAPAIR_H
#define TRAWDATAPAIR_H

#include <TRawDataObject.h>

namespace art {
   class TRawDataPair;
}

class art::TRawDataPair : public TRawDataObject {
public:
   // Default constructor
   TRawDataPair();
   // Default destructor
   ~TRawDataPair();
   // Copy constructor
   TRawDataPair(const TRawDataPair& rhs);
   // Assignment operator
   TRawDataPair& operator=(const TRawDataPair& rhs);

   Int_t& operator[](const Int_t idx);

   void SetFirst(const Int_t &first) { fFirst = first; }
   void SetSecond(const Int_t &second) { fSecond = second; }

   Int_t GetFirst() const { return fFirst; }
   Int_t GetSecond() const { return fSecond; }

   void Swap();

private:
   Int_t fFirst;
   Int_t fSecond;

   ClassDef(TRawDataPair, 1);
};

#endif // TRAWDATAPAIR_H


