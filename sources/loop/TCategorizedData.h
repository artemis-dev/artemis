/* $Id:$ */
/**
 * @file   TCategorizedData.h
 * @date   Created : Jul 16, 2013 17:16:43 JST
 *   Last Modified : Sep 19, 2013 09:18:43 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 * 2013.07.16 Renamed from TCatRawDataCategorized and modified
 *
 *    Copyright (C)2013
 */
#ifndef TCATEGORIZEDDATA_H
#define TCATEGORIZEDDATA_H

#include <TRefArray.h>
#include <TClonesArray.h>

namespace art {
   class TCategorizedData;
   class TRawDataObject;
}

class art::TCategorizedData  : public TObject {

public:
   TCategorizedData();
   ~TCategorizedData();

   virtual TObjArray* GetCategory(const Int_t &idx) {
      return (TObjArray*) fCats->At(idx);
   }

   virtual TObjArray* FindCategory(const Int_t &id) {
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

   virtual void Add(TRawDataObject *obj);
   

private:
   TClonesArray *fCats; //-> array with all categories
   TClonesArray *fDets; //-> array with all detectors
   TClonesArray *fTypes; //-> array with all datatypes

   TObjArray *fCategory; //! cash for the last category

   static TClonesArray *fgCats; //!
   static TClonesArray *fgDets; //!
   static TClonesArray *fgTypes; //!

   
   ClassDef(art::TCategorizedData,1);
};
#endif // end of #ifdef TCATEGORIZEDDATA_H
