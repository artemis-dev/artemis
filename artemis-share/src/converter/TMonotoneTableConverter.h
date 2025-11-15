/**
 * @file   TMonotoneTableConverter.h
 * @brief
 *
 * @date   Created:       2013-10-17 16:46:30
 *         Last Modified: 2014-03-04 12:00:37 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 * @note   you have to put the number of rows of parameter
 *         at the first line of your input file.
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMONOTONETABLECONVERTER_H
#define TMONOTONETABLECONVERTER_H

#include "TConverterBase.h"
#include <vector>

namespace art {
   class TMonotoneTableConverter;
}

class art::TMonotoneTableConverter : public TConverterBase {
public:
   // Default constructor
   TMonotoneTableConverter();
   TMonotoneTableConverter(const TMonotoneTableConverter& rhs);
   virtual ~TMonotoneTableConverter();

   TMonotoneTableConverter& operator=(const TMonotoneTableConverter& rhs);

   virtual Double_t Convert(Double_t val) const;
   virtual void Set(const Double_t *x, const Double_t *y, Int_t nPoints);

   virtual Bool_t LoadTextFile(std::istream &str);

   virtual void Print(Option_t* opt = "") const;

   virtual void PrintDescriptionText() const;

private:
   std::vector<Double_t> *fX; //->
   std::vector<Double_t> *fY; //->
   Int_t fNPoints;

   ClassDef(TMonotoneTableConverter,1) // converter using monotonically in(de)creasing table
};

#endif // TMONOTONETABLECONVERTER_H
