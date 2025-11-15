/**
 * @file   TGateObject.cc
 * @brief
 *
 * @date   Created:       2014-01-16 11:46:12
 *         Last Modified: 2014-06-19 15:29:53 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TGateObject.h"
#include "TGateInfo.h"

#include "constant.h"

using art::TGateObject;

ClassImp(art::TGateObject);

// Default constructor
TGateObject::TGateObject() : fState(kFALSE), fInfo(NULL) {}

TGateObject::~TGateObject() {}

// Copy constructor
TGateObject::TGateObject(const TGateObject& rhs)
   : fState(rhs.fState), fInfo(rhs.fInfo) {}

// Assignment operator
TGateObject& TGateObject::operator=(const TGateObject& rhs)
{
   if (this != &rhs) {
      rhs.Copy(*this);
   }
   return *this;
}

void TGateObject::Copy(TObject &dest) const
{
   TArtemisObject::Copy(dest);
   TGateObject &cobj = *(TGateObject*)&dest;
   cobj.fState = fState;
   cobj.fInfo = fInfo;
}

void TGateObject::Print(Option_t*) const
{
   if (fInfo) {
      printf("OBJ: art::TGateObject\tID: %d\tName: %s\tState: %s\n",
	     fInfo->GetID(),fInfo->GetName(), fState ? "true" : "false");
   } else {
      printf("OBJ: art::TGateObject\tState: %s\n",fState ? "true" : "false");
   }

}
