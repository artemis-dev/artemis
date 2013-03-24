/* $Id:$ */
/**
 * @file   TCatRawDataCategorized.cc
 * @date   Created : Feb 18, 2012 19:18:16 JST
 *   Last Modified : Mar 15, 2013 14:08:54 JST
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
   return (TObjArray*) fCats->ConstructedAt(fCats->GetEntriesFast());
}

TObjArray* TCatRawDataCategorized::AddDet()
{
   return (TObjArray*) fDets->ConstructedAt(fDets->GetEntriesFast());
}

TRefArray* TCatRawDataCategorized::AddType()
{
   return (TRefArray*) fTypes->ConstructedAt(fTypes->GetEntriesFast());
}

