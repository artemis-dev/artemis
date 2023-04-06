/**
 * @file   TWalkConverter.cc
 * @brief  walk correction converter
 *
 * @date   Created       : 2014-06-17 03:08:52 JST
 *         Last Modified : 2014-06-20 16:27:55 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TWalkConverter.h"
#include <TROOT.h>
#include <TClass.h>

using art::TWalkConverter;

ClassImp(TWalkConverter)

TWalkConverter::TWalkConverter() : TFixedNumberParameter(2) {}

TWalkConverter::~TWalkConverter() {}

TWalkConverter::TWalkConverter(const TWalkConverter& rhs) : TFixedNumberParameter(rhs) {}

TWalkConverter& TWalkConverter::operator=(const TWalkConverter& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TWalkConverter::Print(Option_t * /* opt */) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   printf("%s f(x) = %lf / TMath::Power(x,%lf)\n", indent.Data(), fParam[0], fParam[1]);
}
