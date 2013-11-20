/* $Id:$ */
/**
 * @file   TSegmentedData.h
 * @date   Created : Jul 16, 2013 22:16:53 JST
 *   Last Modified : Jul 22, 2013 15:00:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSEGMENTEDDATA_H
#define TSEGMENTEDDATA_H

#include <TClonesArray.h>
namespace art {
   class TSegmentedData;
}

class art::TSegmentedData  : public TClonesArray {

public:
   TSegmentedData();
   virtual ~TSegmentedData();

   
   virtual TObjArray *NewSegment(Int_t id) {
      TObject *obj = ConstructedAt(GetEntriesFast());
      obj->SetUniqueID(id);
      return (TObjArray*) obj;
   }
   virtual TObjArray* FindSegmentByID(Int_t id) {
      const Int_t &n = GetEntriesFast();
      for (Int_t i=0; i!=n; i++) {
         if (At(i)->GetUniqueID() == id) return (TObjArray*)At(i);
      }
      return NULL;
   }

   virtual TObjArray* FindSegment(Int_t dev, Int_t fp, Int_t det) {
      Int_t id;
      id = (dev << 20) + (fp << 14) + (det << 8);
      return FindSegmentByID(id);
   }


   ClassDef(TSegmentedData,1);
};
#endif // end of #ifdef TSEGMENTEDDATA_H
