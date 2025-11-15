/**
 * @file   TAffineConverter.cc
 * @brief
 *
 * @date   Created:       2013-10-14 15:15:43
 *         Last Modified: 2014-06-20 16:17:48 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TAffineConverter.h"

#include <cstdio>
#include <iostream>

#include <TROOT.h>
#include <TString.h>
#include <TClass.h>

using art::TAffineConverter;

ClassImp(art::TAffineConverter)

TAffineConverter::~TAffineConverter() {
}

void TAffineConverter::Print(Option_t *) const {
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   printf("%s f(x) = %lf + %lf * x\n", indent.Data(), fParam[0], fParam[1]);
}
