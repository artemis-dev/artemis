/**
 * @file   TGateArray.cc
 * @brief
 *
 * @date   Created:       2014-01-16 10:27:00
 *         Last Modified: 2014-11-05 11:42:44 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TGateArray.h"

#include <TClonesArray.h>
#include <TList.h>

#include <TEventCollection.h>
#include "TGateObject.h"
#include "TGateInfo.h"

using art::TGateArray;
using art::TGateObject;

ClassImp(art::TGateArray);

// Default constructor
TGateArray::TGateArray() : fSize(0), fInfoList(NULL), fGates(NULL) {}

TGateArray::~TGateArray()
{
   delete fInfoList;
   delete fGates;
}

// Copy constructor
TGateArray::TGateArray(const TGateArray& rhs)
   : fSize(rhs.fSize)
{
   fInfoList = static_cast<TList*>(rhs.fInfoList->Clone());
   fGates = static_cast<TClonesArray*>(rhs.fGates->Clone());
}

// Assignment operator
TGateArray& TGateArray::operator=(const TGateArray& rhs)
{
   if (this != &rhs) {
      fSize = rhs.fSize;
      delete fInfoList;
      delete fGates;
      fInfoList = static_cast<TList*>(rhs.fInfoList->Clone());
      fGates = static_cast<TClonesArray*>(rhs.fGates->Clone());
   }
   return *this;
}

TGateObject& TGateArray::operator[](Int_t gateid)
{
   return const_cast<TGateObject&>(static_cast<const TGateArray&>(*this)[gateid]);
}

const TGateObject& TGateArray::operator[](Int_t gateid) const
{
   return *static_cast<TGateObject*>(fGates->At(gateid));
}

TGateObject** TGateArray::GetObjectRef(const char* name)
{
   return const_cast<TGateObject**>(static_cast<const TGateArray&>(*this).GetObjectRef(name));
}

const TGateObject** TGateArray::GetObjectRef(const char* name) const
{
   const Int_t id = GetID(name);
   const TGateObject *gate = IsValid(id) ? &operator[](id) : NULL;
   return const_cast<const TGateObject**>(
      reinterpret_cast<TGateObject**>(fGates->GetObjectRef(gate)));
}

Bool_t TGateArray::IsTrue(Int_t gateid) const
{
   return (gateid >= 0 && gateid < fSize) ? static_cast<TGateObject*>(fGates->At(gateid))->IsTrue() : kFALSE;
}

Bool_t TGateArray::IsTrue(const char* name) const
{
   return IsTrue(GetID(name));
}

Bool_t TGateArray::IsDefined(const char* name) const
{
   return fInfoList->FindObject(name);
}

Bool_t TGateArray::IsDefined(Int_t id) const
{
   return id >= 0 && id < fSize;
}

Int_t TGateArray::GetID(const char* name) const
{
   if (!fInfoList) return kInvalidI;
   const TGateInfo *const info = dynamic_cast<TGateInfo*>(fInfoList->FindObject(name));
   return info ? info->GetID() : kInvalidI;
}

void TGateArray::Init()
{
   if (fGates) return;
   fSize = 0;
   fInfoList = new TList;
   fInfoList->SetOwner(kTRUE);
   fGates = new TClonesArray(TGateObject::Class());
}

void TGateArray::Reset()
{
   for (Int_t i = 0; i!=fSize; ++i) {
      static_cast<TGateObject*>(fGates->At(i))->SetState(kFALSE);
   }
}

TGateObject* TGateArray::New(const char* name, const TString &description, Int_t)
{
   if (fInfoList->FindObject(name)) { // if 'name' defined
      Error("New","gate named '%s' already defined!",name);
      return NULL;
   }

   TGateInfo *info = new TGateInfo(fSize,name,description); // fSize becomes gateid
   fInfoList->Add(info);
   TGateObject *gate = static_cast<TGateObject*>(fGates->ConstructedAt(fSize++));
   gate->SetInfo(info);
   return gate;
}
