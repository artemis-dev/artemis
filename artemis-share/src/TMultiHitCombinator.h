/**
 * @file   TMultiHitCombinator.h
 * @brief
 *
 * @date   Created:       2013-12-03 14:24:32
 *         Last Modified: 2014-02-24 14:37:39
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMULTIHITCOMBINATOR_H
#define TMULTIHITCOMBINATOR_H

#include "TMultiHitArray.h"

namespace art {
   class TMultiHitCombinator;
}


class art::TMultiHitCombinator : public TMultiHitArray {
public:
   // Default constructor
   TMultiHitCombinator();
   explicit TMultiHitCombinator(Bool_t);
   virtual ~TMultiHitCombinator();

   virtual void Prepare();
   virtual Bool_t Next();
   virtual const TObject* GetHit(UInt_t id) const;
   virtual UInt_t GetNCombination() const;
   virtual UInt_t GetCombinationID() const;
   virtual Bool_t SetCombinationByID(UInt_t cid);

protected:

   Bool_t fPrepared;    // true only after Prepare()
   Bool_t fRunOut;      // true when all the combinations have run out
   Bool_t fComplete;    // true when all the ID have hits.
   Bool_t fAllowNoHit;  // true when all the ID don't have to have hit

   UInt_t  fNID;
   UInt_t *fNHits;         //![fNID]
   UInt_t *fHitIdx;        //![fNID]
   UInt_t  fCombinationID; //!

private:
   Bool_t IncrementIdx(UInt_t);

   // Copy constructor (prohibited)
   TMultiHitCombinator(const TMultiHitCombinator&);
   // Assignment operator (prohibited)
   TMultiHitCombinator& operator=(const TMultiHitCombinator&);

   ClassDef(TMultiHitCombinator,0) // class for generation of multihit combination
};

#endif // TMULTIHITCOMBINATOR_H
