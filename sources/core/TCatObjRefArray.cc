/* $Id:$ */
/**
 * @file   TCatObjRefArray.cc
 * @date   Created : Feb 18, 2012 20:18:40 JST
 *   Last Modified : Feb 19, 2012 02:35:23 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatObjRefArray.h"
#include <TClass.h>

ClassImp(TCatObjRefArray);

TCatObjRefArray::TCatObjRefArray()
{
   
}
TCatObjRefArray::~TCatObjRefArray()
{
}

TObject* TCatObjRefArray::Find(const Int_t &id)
{
//   printf("id = %d, entries = %d, %d\n",fID, fRefs.GetEntries(),fNumEntries);
   TIter next(fRefs.MakeIterator());
   TCatObjRefArray* obj;
   while ((obj = (TCatObjRefArray*) next())) {
      if (obj->IsA() == Class() && 
          obj->GetID() == id) {
         return obj;
      }
   } 
   return NULL;
}
