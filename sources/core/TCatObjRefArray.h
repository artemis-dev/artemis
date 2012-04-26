/* $Id:$ */
/**
 * @file   TCatObjRefArray.h
 * @date   Created : Feb 18, 2012 20:18:32 JST
 *   Last Modified : Feb 19, 2012 16:18:32 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATOBJREFARRAY_H
#define TCATOBJREFARRAY_H

#include <TRefArray.h>

class TCatObjRefArray  : public TObject {

public:
   TCatObjRefArray();
   virtual ~TCatObjRefArray();

   virtual Int_t GetID() { return fID; }
   virtual void  SetID(const Int_t &id) { fID = id; }

   virtual void Add(TObject *obj, Int_t idx = -1) { 
      if (idx != -1) {
         fRefs.AddAtAndExpand(obj,idx);
      } else {
         fRefs.AddAtFree(obj); 
      }
      fNumObj++;
   }

   virtual TObject* At(const Int_t & idx) { 
      return fRefs.At(idx); 
   }

   virtual TObject* operator[] (Int_t idx) const { return fRefs.At(idx); }

   virtual TObject* Find(const Int_t &id);
   
   virtual void Clear(Option_t * /*opt*/) {
      fID = -1;
      fNumObj = 0;
      fRefs.Clear();
      // printf("Clear %d\n",GetEntries());
   }
   
   virtual Int_t GetEntries() { return fNumObj; }
   virtual Int_t GetLast() { return fRefs.GetLast(); }

private:
   Int_t     fID;
   Int_t     fNumObj;
   TRefArray fRefs;

   ClassDef(TCatObjRefArray,1);

};
#endif // end of #ifdef TCATOBJREFARRAY_H
