/**
 * @file   TAffineConverter.h
 * @brief  x -> f(x) = a + b*x type converter
 *
 * @date   Created:       2013-10-14 15:11:37
 *         Last Modified: 2014-06-20 17:14:40 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TAFFINECONVERTER_H
#define TAFFINECONVERTER_H

#include "TFixedNumberParameter.h"

namespace art{
   class TAffineConverter;
}

class TString;

class art::TAffineConverter : public TFixedNumberParameter {
public:
   // Default constructor
   TAffineConverter() : TFixedNumberParameter(2) { fParam[1] = 1.;}
   TAffineConverter(Double_t a0, Double_t a1)
      : TFixedNumberParameter(2) { Set(a0,a1); }

   virtual ~TAffineConverter();

   virtual void Set(Double_t a0, Double_t a1) {fParam[0] = a0; fParam[1] = a1;}
   virtual Double_t Convert(Double_t val) const {return fParam[0] + fParam[1] * val;}
   virtual void Get(Double_t &a0, Double_t &a1) const  {a0 = fParam[0];a1 = fParam[1];}

   virtual void Print(Option_t* opt = "") const;

   ClassDef(TAffineConverter,1) // converter with first-degree polynomial
};

#endif // TAFFINECONVERTER_H
