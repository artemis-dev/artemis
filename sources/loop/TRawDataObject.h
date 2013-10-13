/* $Id:$ */
/**
 * @file   TRawDataObject.h
 * @date   Created : Jul 16, 2013 17:16:51 JST
 *   Last Modified : 
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

   virtual Int_t GetSegID() { return fSegID; }
   virtual Int_t GetGeo() { return fGeo; }
   virtual Int_t GetCh() { return fCh; }

   virtual Int_t GetCatID() { return fCatID; }
   virtual Int_t GetDetID() { return fDetID; }
   virtual Int_t GetType() { return fType; }

   virtual void SetSegID(const Int_t &val) { fSegID = val; }
   virtual void SetGeo(const Int_t &val) { fGeo = val; }
   virtual void SetCh(const Int_t &val) { fCh = val; }

   virtual void SetCatID(const Int_t &val) { fCatID = val; }
   virtual void SetDetID(const Int_t &val) { fDetID = val; }
   virtual void SetType(const Int_t &val) { fType = val; }

   virtual void SetSegInfo(const Int_t &segid,  const Int_t& geo, const Int_t& ch) {
      fSegID = segid;
      fGeo   = geo;
      fCh    = ch;
   }
   virtual void SetCatInfo(const Int_t &catid, const Int_t& detid, const Int_t& type) {
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
