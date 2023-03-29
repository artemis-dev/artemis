/**
 * @file   TFixedNumberParameter.h
 * @brief  converter base with multi-parameter
 *
 * @date   Created       : 2014-06-20 13:43:06 JST
 *         Last Modified : 2018-02-04 11:46:22 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9CCA782E_FFC4_4B03_95D3_0AC443F2F358
#define INCLUDE_GUARD_UUID_9CCA782E_FFC4_4B03_95D3_0AC443F2F358

#include "TConverterBase.h"

namespace art{
   class TFixedNumberParameter;
}

class art::TFixedNumberParameter : public TConverterBase {
public:
   explicit TFixedNumberParameter(UInt_t nPar = 1) : fParam(nPar) {}
   virtual ~TFixedNumberParameter(){}

   TFixedNumberParameter(const TFixedNumberParameter& rhs)
      : TConverterBase(rhs), fParam(rhs.fParam) {};
   TFixedNumberParameter& operator=(const TFixedNumberParameter& rhs);

   virtual void Print(Option_t*) const;
   virtual void PrintDescriptionText() const;

   virtual Bool_t LoadTextFile(std::istream &str);
   virtual Bool_t LoadString(const TString &s);

protected:
   std::vector<Float_t> fParam; // parameter array

   ClassDef(TFixedNumberParameter,1) // parameter with fixed number
};

#endif // INCLUDE_GUARD_UUID_9CCA782E_FFC4_4B03_95D3_0AC443F2F358
