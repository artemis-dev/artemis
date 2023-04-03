/* $Id:$ */
/**
 * @file   TCatScalerData.cc
 * @date   Created : Feb 13, 2013 20:13:31 JST
 *   Last Modified : Oct 31, 2013 06:50:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatScalerData.h"

ClassImp(art::TCatScalerData);

art::TCatScalerData::TCatScalerData(Int_t num)
   : fNumChannel(num)
{
   SetNumChannel(fNumChannel);
}
art::TCatScalerData::~TCatScalerData()
{
   delete [] fData;
}


void art::TCatScalerData::SetNumChannel(Int_t nCh)
{ 
   fNumChannel = nCh;
   fData = new UInt_t[fNumChannel];
}

void art::TCatScalerData::Clear(Option_t * /*opt*/)
{
   for (Int_t i=0; i!=fNumChannel; i++) {
      fData[i] = 0;
   }
}
