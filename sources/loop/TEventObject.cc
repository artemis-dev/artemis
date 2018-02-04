/* $Id:$ */
/**
 * @file   TEventObject.cc
 * @date   Created : Jul 10, 2013 14:10:50 JST
 *   Last Modified : 2016-07-15 10:55:01 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TEventObject.h"
#include <TTree.h>

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

Bool_t art::TEventObject::SetBranch(TTree *tree)
{
   if (!this->IsObject()) {
      // primitivee class
      TString brNme = this->GetName();
      TString leaflist;
      if (this->GetLength().IsNull()) {
         leaflist = TString::Format("%s/%s",this->GetName(),this->GetType());
      } else {
         leaflist = TString::Format("%s[%s]/%s",this->GetName(),this->GetLength().Data(),this->GetType());
      }
      tree->Branch(this->GetName(),*this->GetObjectRef(),leaflist);
   } else {
      tree->Branch(this->GetName(),this->GetClass()->GetName(),this->GetObjectRef(),3200000,0);
   }
   return kTRUE;
}


