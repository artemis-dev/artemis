/* $Id:$ */
/**
 * @file   TCatSortable.cc
 * @date   Created : Feb 04, 2012 21:04:28 JST
 *   Last Modified : Feb 04, 2012 23:14:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSortable.h"

ClassImp(TCatSortable);

TCatSortable::TCatSortable()
{
}

TCatSortable::~TCatSortable()
{
}

TCatSortable::TCatSortable(TArtRawDataObject *obj, const Double_t &val)
   : fObj(obj), fVal(val)
{
}


Int_t TCatSortable::Compare(const TObject *obj) const
{
   const TCatSortable *a = (TCatSortable*) obj;
   if (fVal == a->fVal) return 0;
   if (fVal > a->fVal) return 1;
   return -1;
}

