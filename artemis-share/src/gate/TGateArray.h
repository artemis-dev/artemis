/**
 * @file   TGateArray.h
 * @brief
 *
 * @date   Created:       2014-01-16 10:20:54
 *         Last Modified: 2014-11-05 11:46:08 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TGATEARRAY_H
#define TGATEARRAY_H

#include "TArtemisObject.h"
#include <constant.h>

namespace art {
   class TGateArray;
   class TGateObject;
   class TEventCollection;
}

class TList;
class TClonesArray;

class art::TGateArray : public TArtemisObject {
public:
   // Default constructor
   TGateArray();
   virtual ~TGateArray();
   // Copy constructor
   TGateArray(const TGateArray& rhs);
   // Assignment operator
   TGateArray& operator=(const TGateArray& rhs);

   TGateObject& operator[](Int_t gateid);
   const TGateObject& operator[](Int_t gateid) const;

   TGateObject** GetObjectRef(const char* name);
   const TGateObject** GetObjectRef(const char* name) const;

   Bool_t IsTrue(Int_t gateid) const;
   Bool_t IsTrue(const char* name) const;

   Bool_t IsFalse(Int_t gateid) const {return !IsTrue(gateid);}
   Bool_t IsFalse(const char* name) const {return !IsTrue(name);}

   Bool_t Test(Int_t gateid) const {return IsTrue(gateid);}
   Bool_t Test(const char* name) const {return IsTrue(name);}

   Bool_t IsDefined(const char* name) const;
   Bool_t IsDefined(Int_t id) const;

   Int_t GetID(const char* name) const;
   
   virtual void Init();
   virtual void Reset();
   virtual TGateObject* New(const char* name, const TString &description, Int_t gateid = kInvalidI);

   TList* GetInfoList() const { return fInfoList; }

   Int_t GetSize() const {return fSize;}

protected:
   Int_t         fSize;
   TList        *fInfoList; //!
   TClonesArray *fGates;    //->

private:

   ClassDef(TGateArray,1) // gate array
};

#endif // TGATEARRAY_H
