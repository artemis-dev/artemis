/* $Id:$ */
/**
 * @file   TCategorizedData.cc
 * @date   Created : Jul 17, 2013 16:17:19 JST
 *   Last Modified: 2013-10-13 12:17:30
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCategorizedData.h"

#include <TRawDataObject.h>

ClassImp(art::TCategorizedData);

TClonesArray* art::TCategorizedData::fgCats = 0;
TClonesArray* art::TCategorizedData::fgDets = 0;
TClonesArray* art::TCategorizedData::fgTypes = 0;


art::TCategorizedData::TCategorizedData()
{
   if (!fgCats) fgCats = new TClonesArray("TObjArray",100);
   if (!fgDets) fgDets = new TClonesArray("TObjArray",1000);
   if (!fgTypes) fgTypes = new TClonesArray("TRefArray",10000);

   fCats = fgCats;
   fDets = fgDets;
   fTypes = fgTypes;
   fCategory = NULL;
}
art::TCategorizedData::~TCategorizedData()
{
}



void art::TCategorizedData::Add(TRawDataObject *obj)
{
   // if object is null then return.
   if (!obj) return;
   const Int_t &cid = obj->GetCatID();
   const Int_t &did = obj->GetDetID();
   const Int_t &tid = obj->GetType();
   TObjArray *detector = NULL;
   TRefArray *type     = NULL;

   if (cid == TRawDataObject::kInvalid) return;
   // find or create category if the current category is null or different from the requested one
   if (!fCategory || fCategory->GetUniqueID() != cid) {
      fCategory = FindCategory(cid);
      if (!fCategory) {
         fCategory = (TObjArray*) fCats->ConstructedAt(fCats->GetEntriesFast());
         fCategory->SetUniqueID(cid);
      }
   }

   Int_t nDets = fCategory->GetEntriesFast();
   for (Int_t iDet = 0; iDet != nDets; iDet++) {
      TObject *obj = fCategory->At(iDet);
      if ((Int_t)obj->GetUniqueID() == did) {
         detector = (TObjArray*) obj;
         break;
      }
   }
   // if detector is null, then the detector should be created
   if (!detector) {
      detector = (TObjArray*) fDets->ConstructedAt(fDets->GetEntriesFast());
      detector->SetUniqueID(did);
      fCategory->Add(detector);
   }
   
   // add data to corresponding type
   type = (TRefArray*) detector->At(tid);
   if (type == NULL) {
      type = (TRefArray*) fTypes->ConstructedAt(fTypes->GetEntriesFast());
      type->SetUniqueID(tid);
      detector->AddAtAndExpand(type,tid);
   }
   type->Add(obj);
}
