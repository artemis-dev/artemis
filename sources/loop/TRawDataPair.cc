/**
 * @file   TRawDataPair.cc
 * @brief
 *
 * @date   Created:       2013-07-25 10:44:58
 *         Last Modified: 2013-07-25 11:55:18
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TRawDataPair.h"

ClassImp(art::TRawDataPair);

using art::TRawDataPair;

// Default constructor
TRawDataPair::TRawDataPair() 
   : fFirst(TRawDataObject::kInvalid), fSecond(TRawDataObject::kInvalid){
}

// Default destructor
TRawDataPair::~TRawDataPair() {
}

// Copy constructor
TRawDataPair::TRawDataPair(const TRawDataPair& rhs) 
   : fFirst(rhs.fFirst), fSecond(rhs.fSecond){
}

// Assignment operator
TRawDataPair& TRawDataPair::operator=(const TRawDataPair& rhs) {
   if (this != &rhs) {
      fFirst = rhs.fFirst;
      fFirst = rhs.fSecond;
   }
   return *this;
}

Int_t& TRawDataPair::operator[](const Int_t idx){
   return !idx ? fFirst : fSecond;
}

void TRawDataPair::Swap() {
   Int_t temp = fFirst;
   fFirst = fSecond;
   fSecond = temp;
}


