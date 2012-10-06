/* $Id:$ */
/**
 * @file   TCatRawDataCategorized.h
 * @date   Created : Feb 18, 2012 18:18:06 JST
 *   Last Modified : Oct 02, 2012 15:58:42 JST
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

   virtual TObjArray* AddCat();
   virtual TObjArray* AddDet();
   virtual TRefArray* AddType();

   virtual Int_t GetNumCats() { return fCats->GetEntriesFast(); }
   virtual Int_t GetNumDets() { return fDets->GetEntriesFast(); }
   virtual Int_t GetNumTypes() { return fTypes->GetEntriesFast(); }

   virtual TObjArray* GetCategory(const Int_t &idx) {
      return (TObjArray*) fCats->At(idx);
   }

   virtual TObjArray* GetCategoryById(const Int_t &id) {
      const Int_t &n = fCats->GetEntriesFast();
      for (Int_t i=0; i != n; i++) {
         if ((Int_t)fCats->At(i)->GetUniqueID() == id) {
            return (TObjArray*)fCats->At(i);
         }
      }
      return NULL;
   }

   virtual void Clear(Option_t *opt = "") {
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

   ClassDef(TCatRawDataCategorized,1);
};
#endif // end of #ifdef TCATCATEGORIZED_H
