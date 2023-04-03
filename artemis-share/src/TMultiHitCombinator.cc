/**
 * @file   TMultiHitCombinator.cc
 * @brief
 *
 * @date   Created:       2013-12-03 14:50:09
 *         Last Modified: 2014-02-24 14:57:15
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMultiHitCombinator.h"
#include "constant.h"

#include <TClonesArray.h>

using art::TMultiHitCombinator;

ClassImp(art::TMultiHitCombinator);

// Default constructor
TMultiHitCombinator::TMultiHitCombinator() 
   : fPrepared(kFALSE), fRunOut(kTRUE), fComplete(kTRUE), fAllowNoHit(kFALSE), fNID(0),
     fNHits(NULL), fHitIdx(NULL), fCombinationID(-1) {
}

TMultiHitCombinator::TMultiHitCombinator(Bool_t allowNoHit) 
   : fPrepared(kFALSE), fRunOut(kTRUE), fComplete(kTRUE), fAllowNoHit(allowNoHit), fNID(0),
     fNHits(NULL), fHitIdx(NULL), fCombinationID(-1) {
}

TMultiHitCombinator::~TMultiHitCombinator() {
   delete fNHits;
   delete fHitIdx;
}

void TMultiHitCombinator::Prepare() {

   fNID = GetEntriesFast();
   delete fNHits;
   fNHits = new UInt_t[fNID];
   delete fHitIdx;
   fHitIdx = new UInt_t[fNID];

   fCombinationID = 0;
   fComplete = kTRUE;
   for(UInt_t i = 0; i!=fNID; ++i) {
      fNHits[i] = ((TObjArray*)fDets->At(i))->GetEntriesFast();
      if (!fAllowNoHit && !fNHits[i]) {
	 fComplete = kFALSE;
      }
      fHitIdx[i] = 0;
   }

   fPrepared = kTRUE;
   fRunOut = kFALSE;
}

inline Bool_t TMultiHitCombinator::Next() {
   if(!fComplete || fRunOut) return kFALSE;
   if(!fPrepared) {
      ++fCombinationID;
      return IncrementIdx(0);
   }
   fPrepared = kFALSE;
   return kTRUE;
}

Bool_t TMultiHitCombinator::IncrementIdx(UInt_t idx) {
   if (idx == fNID) {
      fRunOut = kTRUE;
      return kFALSE;
   }

   ++(fHitIdx[idx]);

   if (fHitIdx[idx] >= fNHits[idx]) { // '>' is for the case fNHits[idx] = 0
      fHitIdx[idx] = 0;
      return IncrementIdx(idx+1);
   }
   return kTRUE;
}

inline const TObject* TMultiHitCombinator::GetHit(UInt_t id) const {
   if (!fComplete || fIdMap[id] == TMultiHitArray::kIdxUnregistered) return NULL;
   return fNHits[fIdMap[id]] ? GetHits(id)->At(fHitIdx[fIdMap[id]]) : NULL;
}

inline UInt_t TMultiHitCombinator::GetNCombination() const {
   if (!fComplete) return 0;

   UInt_t nCombination = 1;
   for (UInt_t i = 0; i!=fNID; ++i) { 
      if (fNHits[i]) {
	 nCombination *= fNHits[i];
      }
   }
   return nCombination;
}

inline UInt_t TMultiHitCombinator::GetCombinationID() const {
   return fCombinationID;
}

Bool_t TMultiHitCombinator::SetCombinationByID(UInt_t cid) {
   if (!fComplete || cid >= GetNCombination()) return kFALSE;
   fPrepared = kFALSE;
   fCombinationID = cid;
   for(UInt_t i = 0; i!=fNID;++i) {
      if (!fNHits[i]) continue;
      fHitIdx[i] = cid % fNHits[i];
      cid /= fNHits[i];
   }
   return kTRUE;
}
