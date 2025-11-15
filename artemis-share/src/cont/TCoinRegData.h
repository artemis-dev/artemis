/**
 * @file   TCoinRegData.h
 * @brief
 *
 * @date   Created:       2014-01-15 14:09:28
 *         Last Modified : Jun 16, 2014 05:43:10 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>, Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro and Shinsuke OTA All rights reserved
 */

#ifndef TCOINREGDATA_H
#define TCOINREGDATA_H

#include "TDataObject.h"
#include <vector>
namespace art {
   class TCoinRegData;
}


class art::TCoinRegData : public TDataObject {
public:
   // Default constructor
   TCoinRegData();
   TCoinRegData(const TCoinRegData&);
   virtual ~TCoinRegData();

   TCoinRegData& operator=(const TCoinRegData& rhs);

   virtual Bool_t Test(UInt_t digit) const {return TestQualityBit(1 << digit);}

   virtual void Clear(Option_t *opt = "") {
      fNumBits = 0;
      fBits.clear();
      TDataObject::Clear(opt);
   }
   virtual void Add(Int_t bit) {
      fBits.push_back(bit);
      fNumBits++;
   }
   virtual Int_t UncheckedAt(Int_t idx) { return fBits[idx]; }

protected:
   Int_t  fNumBits;
   std::vector<Int_t> fBits; //[fNumBits]

   ClassDef(TCoinRegData,1)
};

#endif // TCOINREGDATA_H
