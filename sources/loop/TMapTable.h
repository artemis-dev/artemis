/* $Id:$ */
/**
 * @file   TMapTable.h
 * @date   Created : Jul 16, 2013 22:16:52 JST
 *   Last Modified : Nov 23, 2013 01:36:44 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMAPTABLE_H
#define TMAPTABLE_H

#include <TObject.h>
#include <TRawDataObject.h>
#include <TSegmentedData.h>

namespace art {
   class TMapTable;
}

class art::TMapTable {
public:
   TMapTable();
   virtual ~TMapTable();

   virtual void SetSegmentID(Int_t segid) {
      if (!fCurrentTable || fCurrentTable->GetUniqueID() != (UInt_t)segid) {
         fCurrentTable = fMapTable->FindSegmentByID(segid);
         if (!fCurrentTable) fCurrentTable = fMapTable->NewSegment(segid);
      }
   }
   virtual void Map(TRawDataObject *obj);
   virtual Bool_t SetMap(Int_t segid, Int_t geo, Int_t ch,
			 Int_t cat, Int_t det, Int_t type);

protected:
   TSegmentedData *fMapTable;
   TObjArray *fCurrentTable;

   ClassDef(TMapTable,1);
};
#endif // end of #ifdef TMAPTABLE_H
