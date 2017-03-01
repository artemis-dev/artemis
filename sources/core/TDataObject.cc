/* $Id:$ */
/**
 * @file   TDataObject.cc
 * @date   Created : Jan 07, 2014 10:07:26 JST
 *   Last Modified : 2016-08-19 10:33:09 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#include "TDataObject.h"
#include <TClass.h>
#include <iostream>

ClassImp(art::TDataObject)

using art::TDataObject;

TDataObject::TDataObject()
   : fID(0),fAuxID(0), fQuality(kInvalid), fTimestamp(0.)
{
}

TDataObject::TDataObject(const TDataObject &obj)
   : TArtemisObject(obj), fID(obj.fID), fAuxID(obj.fAuxID), fQuality(obj.fQuality), fTimestamp(obj.fTimestamp)
{
}

TDataObject::~TDataObject()
{
}

void TDataObject::Clear(Option_t *option)
{
   fID = fAuxID = 0;
   fTimestamp = 0.;
   fQuality = kInvalid;
   TObject::Clear(option);
}

void TDataObject::Copy(TObject &object) const
{
   TDataObject &obj = *(TDataObject*)&object;
   obj.fID = fID;
   obj.fAuxID = fAuxID;
   obj.fQuality = fQuality;
   obj.fTimestamp = fTimestamp;
   TObject::Copy(obj);
}

void TDataObject::Print(Option_t*) const
{
   std::cout << "OBJ:" << IsA()->GetName() << "\t" << "ID = " << fID << "\t" << "AuxID = " << fAuxID << std::endl;
}

TDataObject& TDataObject::operator=(const TDataObject &rhs)
{
   if (this != &rhs) {
      const TDataObject& obj = *(TDataObject*)&rhs;
      fID = obj.fID;
      fAuxID = obj.fAuxID;
      fQuality = obj.fQuality;
      fTimestamp = obj.fTimestamp;
   }
   return *this;
}


void TDataObject::SetQualityBit(UInt_t quality, Bool_t set)
{
   if (set) {
      SetQualityBit(quality);
   } else {
      ResetQualityBit(quality);
   }
}
