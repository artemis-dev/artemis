/**
 * @file   TPolynomialConverter.h
 * @brief
 *
 * @date   Created:       2013-10-13 15:26:58
 *         Last Modified: 2014-06-19 14:55:41 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TPOLYNOMIALCONVERTER_H
#define TPOLYNOMIALCONVERTER_H

#include "TConverterBase.h"

namespace art{
   class TPolynomialConverter;
}

class art::TPolynomialConverter : public TConverterBase {
public:
   // Default constructor
   TPolynomialConverter();
   virtual ~TPolynomialConverter();

   virtual void Set(const std::vector<Double_t> &val);
   virtual Double_t Convert(Double_t val) const;

   virtual Bool_t LoadTextFile(std::istream &str);
   virtual Bool_t LoadString(const TString &s);

   virtual void Print(Option_t* opt = "") const;

   virtual void PrintDescriptionText() const;

private:
   std::vector<Double_t> *fCoefficient; //->

   ClassDef(TPolynomialConverter,2) // converter with polynomial of arbitrary degree
};

#endif // TPOLYNOMIALCONVERTER_H
