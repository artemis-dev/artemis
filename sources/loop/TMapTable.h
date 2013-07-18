/* $Id:$ */
/**
 * @file   TMapTable.h
 * @date   Created : Jul 16, 2013 22:16:52 JST
 *   Last Modified : 
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
      if (!fCurrentTable || fCurrentTable->GetUniqueID() != segid) {
         fCurrentTable = fMapTable->FindSegmentByID(segid);
         if (!fCurrentTable) fCurrentTable = fMapTable->NewSegment(segid);
      }
   }
   virtual void Map(TRawDataObject *obj);
   virtual Bool_t SetMap(const Int_t &segid, const Int_t &geo, const Int_t &ch,
                       const Int_t &cat, const Int_t &det, const Int_t &type);

protected:
   TSegmentedData *fMapTable;
   TObjArray *fCurrentTable;

   ClassDef(TMapTable,1);
};
#endif // end of #ifdef TMAPTABLE_H
