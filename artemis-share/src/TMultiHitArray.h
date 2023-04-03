/**
 * @file   TMultiHitArray.h
 * @brief
 *
 * @date   Created:       2013-11-13 11:40:38
 *         Last Modified: 2014-05-15 15:31:04 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMULTIHITARRAY_H
#define TMULTIHITARRAY_H

#include "TArtemisObject.h"

namespace art {
   class TMultiHitArray;
   class TDataObject;
}

class TClonesArray;
class TObjArray;

class art::TMultiHitArray : public TArtemisObject {
public:
   // Default constructor
   TMultiHitArray();
   virtual ~TMultiHitArray();

   virtual TObjArray* At(Int_t idx);
   virtual Int_t GetEntriesFast() const ;
   virtual const TObjArray* GetHits(Int_t detID) const;
   virtual void Add(TDataObject *const hit, Int_t id = -100000000);

   virtual void Clear(Option_t* opt = ""); // Clear multihits. DetID information will not be cleared.

protected:
   std::vector<Int_t> fIdMap; //! map of id to idx
   TClonesArray *fDets; // array of dets. each element includes TObjArray (multihit array)
   static const Int_t kIdxUnregistered;

private:
   // Copy constructor (prohibited)
   TMultiHitArray(const TMultiHitArray&);
   // Assignment operator (prohibited)
   TMultiHitArray& operator=(const TMultiHitArray&);

   ClassDef(TMultiHitArray,0) // class for multihit treatment
};

#endif // TMULTIHITARRAY_H
