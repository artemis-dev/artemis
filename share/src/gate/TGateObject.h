/**
 * @file   TGateObject.h
 * @brief
 *
 * @date   Created:       2014-01-16 11:40:05
 *         Last Modified: 2014-06-19 15:22:56 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TGATEOBJECT_H
#define TGATEOBJECT_H

#include "TArtemisObject.h"

namespace art {
   class TGateObject;
   class TGateInfo;
}

class art::TGateObject : public TArtemisObject {
public:
   // Default constructor
   TGateObject();
   virtual ~TGateObject();
   // Copy constructor
   TGateObject(const TGateObject& rhs);
   // Assignment operator
   TGateObject& operator=(const TGateObject& rhs);
   TGateObject& operator=(Bool_t flag) {fState = flag; return *this;}

   void SetState(Int_t state) {fState = state;}

   Bool_t IsTrue() const {return fState;}
   Bool_t IsFalse() const {return !fState;}
   Bool_t Test(Bool_t val = kTRUE) const {return fState == val;}

   void SetInfo(TGateInfo *info) {fInfo = info;}
   const TGateInfo* GetInfo() const {return fInfo;}

   virtual void Clear(Option_t*) {fState = kFALSE;}
   virtual void Copy(TObject &dest) const;

   virtual void Print(Option_t* opt = "") const;

protected:
   Bool_t           fState;
   const TGateInfo* fInfo; //! not owned by this

   ClassDef(TGateObject,1) // Gate object
};

#endif // TGATEOBJECT_H
