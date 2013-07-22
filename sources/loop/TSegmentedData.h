/* $Id:$ */
/**
 * @file   TSegmentedData.h
 * @date   Created : Jul 16, 2013 22:16:53 JST
 *   Last Modified : 
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
      TIter next(this);
      TObject *obj;
      while ((obj = next())) {
         if (obj->GetUniqueID() == id) return (TObjArray*)obj;
      }
      return NULL;
   }


   ClassDef(TSegmentedData,1);
};
#endif // end of #ifdef TSEGMENTEDDATA_H
