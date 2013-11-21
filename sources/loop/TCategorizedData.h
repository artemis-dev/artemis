/* $Id:$ */
/**
 * @file   TCategorizedData.h
 * @date   Created : Jul 16, 2013 17:16:43 JST
 *   Last Modified : Nov 21, 2013 10:51:20 JST
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
         if (!fCats->At(i)) continue;
         if ((Int_t)fCats->At(i)->GetUniqueID() == id) {
            return (TObjArray*)fCats->At(i);
         }
      }
      return NULL;
   }

   virtual void Clear(Option_t *opt = "") {
      TObject::Clear(opt);
      fCats->Clear(opt);
      fgCats->Clear(opt);
      fgDets->Clear(opt);
      fgTypes->Clear(opt);
      
//      fDets->Clear(opt);
//      fTypes->Clear(opt);
      fCategory = NULL;
   }

   virtual void Add(TRawDataObject *obj);
   

protected:
   TObjArray *fCats; //-> array with all categories
//   TObjArray *fDets; //-> array with all detectors
//   TObjArray *fTypes; //-> array with all datatypes

   TObjArray *fCategory; //! cash for the last category

   static TClonesArray *fgCats; //!
   static TClonesArray *fgDets; //!
   static TClonesArray *fgTypes; //!

   
   ClassDef(TCategorizedData,1);
};
#endif // end of #ifdef TCATEGORIZEDDATA_H
