/**
 * @file   TSplineConverter.h
 * @brief  converter using spline
 *
 * @date   Created       : 2015-04-20 12:07:33 JST
 *         Last Modified : 2018-02-04 11:56:31 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_D0155508_4059_4162_ADB4_6C7FC482D723
#define INCLUDE_GUARD_UUID_D0155508_4059_4162_ADB4_6C7FC482D723

#include "TConverterBase.h"

namespace art {
   class TSplineConverter;
}

class TSpline3;

class art::TSplineConverter : public TConverterBase {
public:
   TSplineConverter();
   virtual ~TSplineConverter();

   TSplineConverter(const TSplineConverter& rhs);
   TSplineConverter& operator=(const TSplineConverter& rhs);

   virtual Double_t Convert(Double_t val) const;
   virtual void Set(Double_t *x, Double_t *y, Int_t nPoints);

   virtual Bool_t LoadTextFile(std::istream &str);

   virtual void Print(Option_t *opt = "") const;
   virtual void PrintDescriptionText() const;

private:
   TSpline3 *fSpline; //->

   ClassDef(TSplineConverter,1) // converter using spline
};

#endif // INCLUDE_GUARD_UUID_D0155508_4059_4162_ADB4_6C7FC482D723
