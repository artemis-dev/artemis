/* $Id:$ */
/**
 * @file   TScalerData.cc
 * @date   Created : Feb 13, 2013 20:13:31 JST
 *   Last Modified : 2017-03-01 13:37:53 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2013
 */
#include "TScalerData.h"

#include <TROOT.h>

ClassImp(art::TScalerData);

art::TScalerData::TScalerData()
   : fNumChannel(0), fDate(0), fData(NULL) {}

art::TScalerData::TScalerData(Int_t num)
   : fNumChannel(num), fDate(0), fData(NULL)
{
   SetNumChannel(fNumChannel);
}
art::TScalerData::~TScalerData()
{
   delete [] fData;
}

void art::TScalerData::SetNumChannel(Int_t nCh)
{
   if(nCh < 0) {
      return;
   } else if(nCh != fNumChannel && fData != NULL) {
      delete fData;
   }
   fNumChannel = nCh;
   fData = new UInt_t[fNumChannel];
}

void art::TScalerData::Clear(Option_t * /*opt*/)
{
   for (Int_t i=0; i!=fNumChannel; ++i) {
      fData[i] = 0;
   }
}

void art::TScalerData::Print(Option_t * /*opt*/) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("art::TScalerData name='%s' title='%s' date=%lu\n",
	  GetName(),GetTitle(),fDate);
   for (Int_t i=0; i!=fNumChannel; ++i) {
      if(!fData[i]) continue;
      printf("%s [%2d] %10d\n",
	     indent.Data(),i,fData[i]);
   }
}

