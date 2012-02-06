/* $Id:$ */
/**
 * @file   TCatUHodo.cc
 * @date   Created : Feb 04, 2012 17:04:15 JST
 *   Last Modified : Feb 05, 2012 11:41:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatUHodo.h"

ClassImp(TCatUHodo);

TCatUHodo::TCatUHodo()
   : TCatDetectorArray()
{
   Init();
}
TCatUHodo::~TCatUHodo()
{
}

void TCatUHodo::Init()
{
   fNumHits = 0;
   fHits = new TClonesArray(TCatUHodoPl::Class());
   printf("TCatUHodo:fHit = %p\n",fHits);
}

void TCatUHodo::AddHit(const Int_t &id, 
                       const Double_t &time, const Double_t &charge)
{
   TCatUHodoPl *hit = new ((*fHits)[fNumHits++]) TCatUHodoPl;
   hit->SetDetectorID(id);
   hit->SetTime(time);
   hit->SetCharge(charge);
}

TCatUHodoPl* TCatUHodo::GetHit(const Int_t &idx)
{
   return (TCatUHodoPl*) fHits->At(idx);
}
