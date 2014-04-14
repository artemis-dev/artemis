/* $Id:$ */
/**
 * @file   TMapTable.cc
 * @date   Created : Jul 16, 2013 23:16:31 JST
 *   Last Modified : Nov 23, 2013 01:37:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TMapTable.h"

#include <TObjString.h>
#include <TConfigFile.h>

art::TMapTable::TMapTable()
{
   fMapTable = new TSegmentedData;
   fCurrentTable = NULL;
}
art::TMapTable::~TMapTable()
{
   delete fMapTable;
}

void art::TMapTable::Map(TRawDataObject *obj)
{
   if (!fCurrentTable || fCurrentTable->GetUniqueID() != (UInt_t)obj->GetSegID()) {
      fCurrentTable = fMapTable->FindSegmentByID(obj->GetSegID());
   }
   if (!fCurrentTable) {
      // if there is no mapping, just skip. This is not always error.
      return;
   }
   const Int_t &geo = obj->GetGeo();
   const Int_t &ch  = obj->GetCh();
   TObjArray *geomap = NULL;
   TRawDataObject *map  = NULL;

   if (fCurrentTable->GetEntriesFast() > geo) {
      geomap = (TObjArray*) fCurrentTable->At(geo);
   }
   if (!geomap) return;
   if (geomap->GetEntriesFast() > ch) {
      map = (TRawDataObject*)geomap->At(ch);
   }
   if (!map) return;
   obj->SetCatInfo(map->GetCatID(),map->GetDetID(),map->GetType());
}


Bool_t  art::TMapTable::SetMap(Int_t segid, Int_t geo, Int_t ch,
			       Int_t cat, Int_t det, Int_t type)
{
   // Set current table to be segid
   SetSegmentID(segid);
   TObjArray *geomap = NULL;
   if (fCurrentTable->GetEntriesFast() > geo) {
      geomap = (TObjArray*)fCurrentTable->At(geo);
   }
   if (!geomap) {
      geomap = new TObjArray;
      fCurrentTable->AddAtAndExpand(geomap,geo);
   }
   if (geomap->GetEntriesFast() > ch) {
      TRawDataObject *old = (TRawDataObject*) geomap->At(ch);
      if (old) {
         printf("Map already exists\n");
         printf("Segment %x, geo = %d, ch = %d\n",segid,geo,ch);
         printf("  Old: cat = %d, det = %d, type = %d\n",
                old->GetCatID(),old->GetDetID(),old->GetType());
         printf("  New: cat = %d, det = %d, type = %d\n",cat,det,type);
         printf("Abort\n");
         return kFALSE;
      }
   }
   TRawDataObject *obj = new TRawDataObject;
   obj->SetSegInfo(segid,geo,ch);
   obj->SetCatInfo(cat,det,type);
   geomap->AddAtAndExpand(obj,ch);
   return kTRUE;
}
