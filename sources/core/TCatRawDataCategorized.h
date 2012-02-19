/* $Id:$ */
/**
 * @file   TCatRawDataCategorized.h
 * @date   Created : Feb 18, 2012 18:18:06 JST
 *   Last Modified : Feb 19, 2012 02:35:05 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATRAWDATACATEGORIZED_H
#define TCATRAWDATACATEGORIZED_H

#include <TCatObjRefArray.h>
#include <TClonesArray.h>

class TCatRawDataCategorized  : public TNamed {

public:
   TCatRawDataCategorized();
   ~TCatRawDataCategorized();

   virtual TCatObjRefArray* AddCat();
   virtual TCatObjRefArray* AddDet();
   virtual TCatObjRefArray* AddType();

   virtual Int_t GetNumCats() { return fNumCats; }
   virtual Int_t GetNumDets() { return fNumDets; }
   virtual Int_t GetNumTypes() { return fNumTypes; }

   virtual TCatObjRefArray* GetCategory(const Int_t &idx) {
      return (TCatObjRefArray*) fCats->At(idx);
   }

   virtual TCatObjRefArray* GetCategoryById(const Int_t &id) {
      for (Int_t i=0; i<fNumCats; i++) {
         if (((TCatObjRefArray*)fCats->At(i))->GetID() == id) {
            return ((TCatObjRefArray*)fCats->At(i));
         }
      }
      return NULL;
   }


   virtual void Clear(Option_t *opt = "") {
      fNumCats = fNumDets = fNumTypes = 0;
      fCats->Clear(opt);
      fDets->Clear(opt);
      fTypes->Clear(opt);
   }


private:
   TClonesArray *fCats; //-> array with all categories
   TClonesArray *fDets; //-> array with all detectors
   TClonesArray *fTypes; //-> array with all datatypes

   static TClonesArray *fgCats; //!
   static TClonesArray *fgDets; //!
   static TClonesArray *fgTypes; //!

   Int_t fNumCats;
   Int_t fNumDets;
   Int_t fNumTypes;

   ClassDef(TCatRawDataCategorized,1);
};
#endif // end of #ifdef TCATCATEGORIZED_H
