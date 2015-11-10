/* $Id:$ */
/**
 * @file   TDataObject.h
 * @date   Created : Jan 07, 2014 10:07:51 JST
 *   Last Modified : Nov 10, 2015 16:11:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TDATAOBJECT_H
#define TDATAOBJECT_H

#include <TArtemisObject.h>

namespace art {
   class TDataObject;
}

class art::TDataObject  : public TArtemisObject {

public:
   static const UInt_t kInvalid = (1<<31);
   
   TDataObject();
   TDataObject(const TDataObject &obj);
   virtual ~TDataObject();

   //=====
   // overridden methods
   //=====
   virtual void Clear(Option_t *option = "");
   virtual void Copy(TObject& object) const;
   virtual void Print(Option_t* option = "") const;
   TDataObject&  operator= (const TDataObject& rhs);

   virtual void SetID(Int_t id) { fID = id; }
   virtual Int_t GetID() const { return fID; }

   virtual void SetAuxID(Int_t auxid) { fAuxID = auxid; }
   virtual Int_t GetAuxID() const { return fAuxID; }

   virtual void SetTimestamp(Double_t time) { fTimestamp = time; }
   virtual Double_t GetTimestamp() { return fTimestamp; }

   virtual void SetQualityBit(UInt_t quality, Bool_t set);
   virtual void SetQualityBit(UInt_t quality) { fQuality |= quality; }
   virtual void ResetQualityBit(UInt_t quality) { fQuality &= (~quality); }
   virtual Bool_t TestQualityBit(UInt_t bit) const { return (Bool_t)((fQuality & bit) != 0); }
   virtual Int_t TestQualityBits(UInt_t bit) const { return (Int_t)(fQuality & bit); }

   virtual Bool_t IsValid() const { return !TestQualityBit(kInvalid); }
   virtual void Validate() { ResetQualityBit(kInvalid); }
   virtual void Invalidate() { SetQualityBit(kInvalid); }

   virtual Bool_t IsSortable() const { return kTRUE; }
   virtual Int_t  Compare(const TObject* obj) const {
      const TDataObject data = *(TDataObject*)obj;
      if (fID == data.fID) return 0;
      if (fID < data.fID) return -1;
      return 1;
   }


private:
   Int_t fID; // identifier of this object artemis never use this id
   Int_t fAuxID; // auxiliary identifier of this object
   UInt_t fQuality; // quality bit
   Double_t fTimestamp; // timestamp
   
   ClassDef(TDataObject,1); // base class for data object
};
#endif // end of #ifdef TDATAOBJECT_H
