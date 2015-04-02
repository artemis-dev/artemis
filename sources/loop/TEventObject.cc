/* $Id:$ */
/**
 * @file   TEventObject.cc
 * @date   Created : Jul 10, 2013 14:10:50 JST
 *   Last Modified : Feb 06, 2015 05:33:41 JST
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
   : TNamed(name,name), fP(obj),fIsPassive(isPassive), fClass(NULL), fType(""), fLength(NULL), fIsObject(kTRUE)
{
   if (obj) fClass = obj->IsA();
}

art::TEventObject::TEventObject(const char* name, void *obj, TString type, TString *length, Bool_t isPassive)
   : TNamed(name,name), fP(obj),fIsPassive(isPassive),
     fClass(NULL), fType(type), fLength(length), fIsObject(kFALSE)
{

}


