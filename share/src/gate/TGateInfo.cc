/**
 * @file   TGateInfo.cc
 * @brief
 *
 * @date   Created:       2014-01-16 12:15:54
 *         Last Modified: 2014-06-19 15:16:57 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TGateInfo.h"

#include <iostream>

#include <TClass.h>

using art::TGateInfo;

// Default constructor
TGateInfo::TGateInfo() : fID(-1) {}

TGateInfo::TGateInfo(Int_t gateid, const TString &name, const TString &description)
   : TNamed(name,description), fID(gateid) {}

TGateInfo::~TGateInfo() {}

// Copy constructor
TGateInfo::TGateInfo(const TGateInfo& rhs)
   : TNamed(rhs), fID(rhs.fID) {}

// Assignment operator
TGateInfo& TGateInfo::operator=(const TGateInfo& rhs)
{
   if (this != &rhs) {
      rhs.TNamed::Copy(*this);
      fID = rhs.fID;
   }
   return *this;
}

void TGateInfo::Print(Option_t* opt) const
{
   if (strcmp(opt,"listg")){
      printf("OBJ: art::TGateInfo\tID:");
   }
   std::cout << fID << "\t"
	     << GetName() << "\t"
	     << GetTitle() << std::endl;
}
