/* $Id:$ */
/**
 * @file   TEventCollection.cc
 * @date   Created : Jul 10, 2013 17:10:20 JST
 *   Last Modified : May 11, 2014 11:54:08 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TEventCollection.h"

using art::TEventCollection;


TEventCollection::TEventCollection()
{
   fObjects = new TList;
   fUserInfo = new TList;
}
TEventCollection::~TEventCollection()
{
   delete fObjects;
   delete fUserInfo;
}


void TEventCollection::Add(const char* name, TObject* obj, Bool_t isPassive)
{
   fObjects->Add(new TEventObject(name,obj,isPassive));
}

void TEventCollection::AddInfo(const char *name, TObject *obj, Bool_t isPassive)
{
   fUserInfo->Add(new TEventObject(name,obj,isPassive));
}

art::TEventObject* TEventCollection::Get(const char* name)
{
   return (TEventObject*) fObjects->FindObject(name);
}

void** TEventCollection::GetObjectRef(const char *name) {
   TObject *obj = NULL;
   if (!(obj = fObjects->FindObject(name))) {
      return NULL;
   }
   return (void**)((TEventObject*)obj)->GetObjectRef();
}

TObject* TEventCollection::GetInfo(const char *name) {
   TObject *obj = NULL;
   if (!(obj = fUserInfo->FindObject(name))) {
      return NULL;
   }
   return *(TObject**)((TEventObject*)obj)->GetObjectRef();
}   


void art::TEventCollection::Delete()
{
   fObjects->Delete();
   fUserInfo->Delete();
}
