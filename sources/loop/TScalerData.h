/* $Id:$ */
/**
 * @file   TScalerData.h
 * @date   Created : Feb 12, 2013 05:12:04 JST
 *   Last Modified : 2017-03-01 13:37:19 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Imported and renamed from TCatScalerData
 *
 *    Copyright (C)2013
 */

#ifndef INCLUDE_GUARD_UUID_5EB690BC_EC5E_4FEE_B01A_D289A162DB8A
#define INCLUDE_GUARD_UUID_5EB690BC_EC5E_4FEE_B01A_D289A162DB8A

#include <TNamed.h>

namespace art {
  class TScalerData;
}

class art::TScalerData  : public TNamed {

public:
   TScalerData();
   explicit TScalerData(Int_t numChannel);
   virtual ~TScalerData();

   void SetNumChannel(Int_t fNumChannel);
   void SetDate(time_t date) { fDate = date; }
   time_t GetDate() const { return fDate; }
   void SetValue(Int_t idx, UInt_t data) { if (idx >= 0 && idx < fNumChannel) fData[idx] = data; }
   UInt_t GetValue(Int_t idx) const {
      if (idx >= 0 && idx < fNumChannel) return fData[idx];
      return 0;
   }
   virtual void Clear(Option_t *opt = "");
   virtual void Print(Option_t*) const;

private:
   Int_t    fNumChannel;
   time_t   fDate;   //epoch time
   UInt_t  *fData;   //[fNumChannel]

   ClassDef(TScalerData,1);
};
#endif // INCLUDE_GUARD_UUID_5EB690BC_EC5E_4FEE_B01A_D289A162DB8A
