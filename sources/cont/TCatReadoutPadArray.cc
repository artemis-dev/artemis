/* $Id:$ */
/**
 * @file   TCatReadoutPadArray.cc
 * @date   Created : Dec 01, 2013 01:01:31 JST
 *   Last Modified : 2016-11-11 18:07:09 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatReadoutPadArray.h"
#include "TCatReadoutPad.h"
#include <TH2Poly.h>
ClassImp(art::TCatReadoutPadArray);

using art::TCatReadoutPadArray;


TCatReadoutPadArray::TCatReadoutPadArray()
{
}
TCatReadoutPadArray::~TCatReadoutPadArray()
{
}

TH2Poly *TCatReadoutPadArray::CreateHist(const char *name, const char *title,
                                         Double_t xmin, Double_t xmax,
                                         Double_t ymin, Double_t ymax,
                                         Bool_t doFillID)
{
   TH2Poly* hist = new TH2Poly(name,title,xmin,xmax,ymin,ymax);
   Int_t nPads = GetEntriesFast();
   for (Int_t iPad = 0; iPad != nPads; iPad++) {
      TCatReadoutPad *pad = (TCatReadoutPad*) At(iPad);
      hist->AddBin(pad->GetNumPoints(),pad->VertexX(),pad->VertexY());
   }
   if (doFillID) {
      for (Int_t iPad = 0; iPad != nPads; iPad++) {
         TCatReadoutPad *pad = (TCatReadoutPad*) At(iPad);
         hist->Fill(pad->X(),pad->Y(),iPad);
      }
   }
   return hist;
}

void TCatReadoutPadArray::SetBads(const std::vector<Int_t>& bads)
{
   std::copy(bads.begin(),bads.end(),back_inserter(fBads));
}

void TCatReadoutPadArray::SetLowGains(const std::vector<Int_t>& lowGains)
{
   std::copy(lowGains.begin(),lowGains.end(),back_inserter(fLowGains));
}


      
