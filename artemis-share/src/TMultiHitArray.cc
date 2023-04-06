/**
 * @file   TMultiHitArray.cc
 * @brief
 *
 * @date   Created:       2013-11-13 11:46:47
 *         Last Modified: 2015-04-10 18:27:23 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMultiHitArray.h"

#include "TDataObject.h"
#include "constant.h"

#include <TClonesArray.h>
#include <TObjArray.h>

using art::TMultiHitArray;

ClassImp(art::TMultiHitArray)

const Int_t TMultiHitArray::kIdxUnregistered = -1;

// Default constructor
TMultiHitArray::TMultiHitArray() {
   fDets = new TClonesArray("TObjArray",16);
   fIdMap.resize(1000,kIdxUnregistered);
}

TMultiHitArray::~TMultiHitArray() {
   delete fDets;
}

inline const TObjArray* TMultiHitArray::GetHits(const Int_t detID) const {
   return fIdMap[detID] != kIdxUnregistered ? (TObjArray*) fDets->At(fIdMap[detID]) : NULL;
}

inline Int_t TMultiHitArray::GetEntriesFast() const {
   return fDets->GetEntriesFast();
}

inline TObjArray* TMultiHitArray::At(Int_t idx) {
   return (TObjArray*)fDets->At(idx);
}

void TMultiHitArray::Add(TDataObject *const hit, Int_t id) {
   if (!hit) return;
   const Int_t detID = (id == kInvalidI) ? hit->GetID() : id;
   if (!IsValid(detID) || detID < 0) return;

   if (fIdMap.size() - 1 < (UInt_t)detID) fIdMap.resize(detID*2,kIdxUnregistered);

   TObjArray *det = NULL;
   if (fIdMap[detID] == kIdxUnregistered) {
      const Int_t n = fDets->GetEntriesFast();
      det = (TObjArray*)fDets->ConstructedAt(n);
      fIdMap[detID] = n;
   } else {
      det = (TObjArray*)fDets->At(fIdMap[detID]);
   }

   det->Add(hit);
}

void TMultiHitArray::Clear(Option_t*) {
   const Int_t n = fDets->GetEntriesFast();
   for (Int_t i = 0; i!=n; ++i) {
      fDets->At(i)->Clear();
   }
}
