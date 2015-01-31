/* $Id:$ */
/**
 * @file   TEventObject.cc
 * @date   Created : Jul 10, 2013 14:10:50 JST
 *   Last Modified : Jan 31, 2015 19:31:05 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TEventObject.h"

art::TEventObject::TEventObject()
   : fP(NULL)
{
}

art::TEventObject::~TEventObject()
{
}

art::TEventObject::TEventObject(const char* name, TObject *obj, Bool_t isPassive)
   : TNamed(name,name), fP(obj),fIsPassive(isPassive)
{
   if (obj) fClass = obj->IsA();
}


art::TEventObject::TEventObject(const char* name, Int_t *obj, Bool_t isPassive)
   : TNamed(name,name), fP(obj),fIsPassive(isPassive),fClass(NULL)
{
   fType = "I";
}

art::TEventObject::TEventObject(const char* name, Double_t *obj, Bool_t isPassive)
   : TNamed(name,name), fP(obj),fIsPassive(isPassive),fClass(NULL)
{
   fType = "D";
}

art::TEventObject::TEventObject(const char* name, Float_t *obj, Bool_t isPassive)
   : TNamed(name,name), fP(obj),fIsPassive(isPassive),fClass(NULL)
{
   fType = "F";
}
