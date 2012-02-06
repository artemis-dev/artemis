/* $Id:$ */
/**
 * @file   TCatUHodoPl.cc
 * @date   Created : Feb 04, 2012 17:04:27 JST
 *   Last Modified : Feb 05, 2012 02:45:03 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatUHodoPl.h"


ClassImp(TCatUHodoPl);

TCatUHodoPl::TCatUHodoPl()
{
}
TCatUHodoPl::~TCatUHodoPl()
{
}
 

Int_t TCatUHodoPl::Compare(const TObject *obj) const 
{
   if (obj == NULL) return 1;
   TCatUHodoPl *pl = (TCatUHodoPl*) obj;
   if (pl->fCharge < this->fCharge) return -1;
   if (pl->fCharge > this->fCharge) return 1;
   return 0;
}
