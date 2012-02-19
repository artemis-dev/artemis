/* $Id:$ */
/**
 * @file   TCatRawDataCategorized.cc
 * @date   Created : Feb 18, 2012 19:18:16 JST
 *   Last Modified : Feb 18, 2012 22:00:46 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatRawDataCategorized.h"

TClonesArray* TCatRawDataCategorized::fgCats = 0;
TClonesArray* TCatRawDataCategorized::fgDets = 0;
TClonesArray* TCatRawDataCategorized::fgTypes = 0;

TCatRawDataCategorized::TCatRawDataCategorized()
{
   if (!fgCats) fgCats = new TClonesArray("TCatObjRefArray",100);
   if (!fgDets) fgDets = new TClonesArray("TCatObjRefArray",1000);
   if (!fgTypes) fgTypes = new TClonesArray("TCatObjRefArray",10000);

   fCats = fgCats;
   fDets = fgDets;
   fTypes = fgTypes;
   fNumCats = 0;
   fNumDets = 0;
   fNumTypes = 0;
}

TCatRawDataCategorized::~TCatRawDataCategorized()
{
}


TCatObjRefArray* TCatRawDataCategorized::AddCat()
{
   TClonesArray &arr = *fCats;
   return new (arr[fNumCats++]) TCatObjRefArray;
}

TCatObjRefArray* TCatRawDataCategorized::AddDet()
{
   TClonesArray &arr = *fDets;
   return new (arr[fNumDets++]) TCatObjRefArray;
}

TCatObjRefArray* TCatRawDataCategorized::AddType()
{
   TClonesArray &arr = *fTypes;
   return new (arr[fNumTypes++]) TCatObjRefArray;
}

