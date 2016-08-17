/* $Id:$ */
/**
 * @file   TRawDataObject.h
 * @date   Created : Jul 16, 2013 17:16:51 JST
 *   Last Modified : 2016-08-17 14:41:38 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRAWDATAOBJECT_H
#define TRAWDATAOBJECT_H

#include <TObject.h>

namespace art
{
   class TRawDataObject;
}

class art::TRawDataObject  : public TObject {

public:

   static const Int_t kInvalid;
   TRawDataObject();
   virtual ~TRawDataObject();

   virtual Int_t GetSegID() const { return fSegID; }
   virtual Int_t GetGeo() const { return fGeo; }
   virtual Int_t GetCh() const { return fCh; }

   virtual Int_t GetCatID() const { return fCatID; }
   virtual Int_t GetDetID() const { return fDetID; }
   virtual Int_t GetType() const { return fType; }

   virtual Double_t GetValue(Int_t /* idx */ = 0) const { return 0; }
   virtual Int_t GetNumValues() const { return 0; }
         

   virtual void SetSegID(Int_t val) { fSegID = val; }
   virtual void SetGeo(Int_t val) { fGeo = val; }
   virtual void SetCh(Int_t val) { fCh = val; }

   virtual void SetCatID(Int_t val) { fCatID = val; }
   virtual void SetDetID(Int_t val) { fDetID = val; }
   virtual void SetType(Int_t val) { fType = val; }

   virtual void SetSegInfo(Int_t segid,  Int_t geo, Int_t ch) {
      fSegID = segid;
      fGeo   = geo;
      fCh    = ch;
   }
   virtual void SetCatInfo(Int_t catid, Int_t detid, Int_t type) {
      fCatID = catid;
      fDetID = detid;
      fType  = type;
   }

   virtual void Clear(Option_t *opt="") {
      TObject::Clear(opt);
      fSegID = kInvalid;
      fGeo   = kInvalid;
      fCh    = kInvalid;
      fCatID = kInvalid;
      fDetID = kInvalid;
      fType  = kInvalid;
   }

protected:
   Int_t fSegID; // segment id 
   Int_t fGeo; // geographical address or virtual station number
   Int_t fCh;  // channel

   Int_t fCatID; // detector category
   Int_t fDetID; // detector id
   Int_t fType; // data type

   ClassDef(TRawDataObject,1);
};
#endif // end of #ifdef TRAWDATAOBJECT_H
