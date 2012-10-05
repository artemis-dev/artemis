/* $Id:$ */
/**
 * @file   TCatRawDataCategorized.cc
 * @date   Created : Feb 18, 2012 19:18:16 JST
 *   Last Modified : Oct 02, 2012 15:53:01 JST
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
   if (!fgCats) fgCats = new TClonesArray("TObjArray",100);
   if (!fgDets) fgDets = new TClonesArray("TObjArray",1000);
   if (!fgTypes) fgTypes = new TClonesArray("TRefArray",10000);

   fCats = fgCats;
   fDets = fgDets;
   fTypes = fgTypes;
}

TCatRawDataCategorized::~TCatRawDataCategorized()
{
}


TObjArray* TCatRawDataCategorized::AddCat()
{
   TClonesArray &arr = *fCats;
   return new (arr[arr.GetEntriesFast()]) TObjArray;
}

TObjArray* TCatRawDataCategorized::AddDet()
{
   TClonesArray &arr = *fDets;
   return new (arr[arr.GetEntriesFast()]) TObjArray;
}

TRefArray* TCatRawDataCategorized::AddType()
{
   TClonesArray &arr = *fTypes;
   return new (arr[arr.GetEntriesFast()]) TRefArray;
}

