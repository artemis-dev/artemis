/* $Id:$ */
/**
 * @file   TCatAtSiHitPatternProcessor.cc
 * @date   Created : Feb 10, 2013 10:10:17 JST
 *         Modified : Oct 19, 2013 00:53:00 JST
 *    Last Modified : 2014-05-15 15:09:46 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 * @modifier Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp> 
 *  
 *    Copyright (C)2013
 */
#include "TCatAtSiHitPatternProcessor.h"
#include <TCategorizedData.h>
#include <TH2Poly.h>
#include <TMath.h>
#include <TROOT.h>
#include <TCanvas.h>
#include "TCatFadcRawData.h"
#include <unistd.h>

art::TCatAtSiHitPatternProcessor::TCatAtSiHitPatternProcessor()
{
   StringVec_t defInputAt;
   StringVec_t defInputSi;
   defInputAt.push_back("atraw");
   defInputSi.push_back("siraw");
   RegisterInputCollection("InputCollectionAt","flash adc raw data",
                           fInputColNameAt,defInputAt);
   RegisterInputCollection("InputCollectionSi","flash adc raw data",
                           fInputColNameSi,defInputSi);
   // printf("Input collection At: %s\n", fInputColNameAt.front());
   // printf("Input collection Si: %s\n", fInputColNameSi.front());
   RegisterProcessorParameter("HistName","HistName",fHistName,TString("hHitPattern"));
}
art::TCatAtSiHitPatternProcessor::~TCatAtSiHitPatternProcessor()
{
}

//TCanvas *canvas;
void art::TCatAtSiHitPatternProcessor::Init(art::TEventCollection *col) {
   if (col->Get(fInputColNameAt.front())) {
      fFADCAT = (TClonesArray**)(col->Get(fInputColNameAt.front())->GetObjectRef());
   }
   if (col->Get(fInputColNameSi.front())) {
      fFADCSI = (TClonesArray**)(col->Get(fInputColNameSi.front())->GetObjectRef());
   }
   printf("Input collection At: %s\n", fInputColNameAt.front().Data());
   printf("Input collection Si: %s\n", fInputColNameSi.front().Data());
   fHPad = DrawPad(fNPad,fPadX,fPadY,fNStrip,fStripX,fStripZ);
   fHPadEvt = DrawPad(fNPad,fPadX,fPadY,fNStrip,fStripX,fStripZ);
   fHPadEvt->SetName(fHistName);
//   col->Add(fHPad,fOutputIsTransparent);
//   fCanvas = (TCanvas*)gROOT->FindObjectAny("c1");
   gDirectory->Add(fHPad);
   gDirectory->Add(fHPadEvt);
   canvas = new TCanvas;
}

void art::TCatAtSiHitPatternProcessor::Process()
{
   Int_t nHitAt = (*fFADCAT)->GetEntries();
   Int_t nHitSi = (*fFADCSI)->GetEntries();
//   fHPad->ClearBinContents();
   fHPadEvt->ClearBinContents();
   // CAT hit
   for (Int_t iHitAt = 0; iHitAt != nHitAt; iHitAt++) {
      TCatFadcRawData* dat = (TCatFadcRawData*)(**fFADCAT)[iHitAt];
      Int_t idat = dat->GetID();
      fHPad->Fill(fPadX[idat],fPadY[idat]);
      fHPadEvt->Fill(fPadX[idat],fPadY[idat]);
   }
   // Si hit 
   for (Int_t iHitSi = 0; iHitSi != nHitSi; iHitSi++) {
      TCatFadcRawData* dsi = (TCatFadcRawData*)(**fFADCSI)[iHitSi];
      Int_t idsi = dsi->GetID();
      fHPad->Fill(fStripX[idsi],fStripZ[idsi]);
      fHPadEvt->Fill(fStripX[idsi],fStripZ[idsi]);
   }
   canvas->cd();
   fHPadEvt->Draw();
   canvas->Update();
   canvas->Modified();
   usleep(500000);
//   sleep(1);
}


TH2Poly* art::TCatAtSiHitPatternProcessor::DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY, Int_t &nStrips, Double_t *&StripX, Double_t *&StripZ) 
{

  // create CAT PAD geometry
   const double deg = TMath::Pi()/180.;
   Double_t l = 10. * sqrt(3)/2. * 2/3.;
   Double_t r =  9. * sqrt(3)/2. * 2/3.;
   Double_t ls = 5. * sqrt(3)/2. * 2/3.;
   Double_t rs = 4. * sqrt(3)/2. * 2/3.;
   Int_t iPads = 0;
   
   nPads = 400;
   padX = new Double_t[400];
   padY = new Double_t[400];
   TH2Poly *hPad = new TH2Poly("hPad","hPad",-200.,200.,-200.,200.);
   for (Int_t iCol = 0; iCol < 6; iCol ++) {
      for (Int_t i=0; i<10; i++) {
         Double_t x[4] = { r*cos(-30*deg),
                           r*cos(90*deg),
                           r*cos(210*deg),
                           r*cos(-30.*deg) };
         Double_t y[4] = { r*sin(-30*deg),
                           r*sin(90*deg),
                           r*sin(210*deg),
                           r*sin(-30.*deg) };
         for (Int_t j=0;j<4;j++) {
            padX[iPads] = (iCol+0.5)*5.-47.5;
            x[j] += padX[iPads];
            if ((iCol%2) == 0) {
               if ((i+1)%2) {
                  padY[iPads] = -l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
               } else {
                  padY[iPads] = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
               }
            } else {
               if ((i)%2) {
                  padY[iPads] = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
               } else {
                  padY[iPads] =-l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
               }
            }
         }
         iPads++;
         hPad->AddBin(4,x,y);
      }
   }
   for (Int_t iCol = 0; iCol < 15; iCol ++) {
      for (Int_t i=0; i<20; i++) {
         Double_t x[4] = { rs*cos(-30*deg),
                           rs*cos(90*deg),
                           rs*cos(210*deg),
                           rs*cos(-30.*deg) };
         Double_t y[4] = { rs*sin(-30*deg),
                           rs*sin(90*deg),
                           rs*sin(210*deg),
                           rs*sin(-30.*deg) };
         if (iCol == 0 || iCol == 14) {
            if ((i%4) == 0 || (i%4) == 3) continue;
         }
         for (Int_t j=0;j<4;j++) {
            padX[iPads] = 5.*((Double_t)iCol)/2.-17.5;
            x[j] += padX[iPads];
            if ((iCol%2) != 0) {
               if ((i+1)%2) {
                  padY[iPads] = -ls/2.+(ls*3.)*(i/2-4.5);
                  y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.5) ;
               } else {
                  padY[iPads] = ls/2.+(ls*3.)*(i/2-4.5);
                  y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 4.5) ;
               }
            } else {
               if ((i+1)%2) {
                  padY[iPads] = ls/2.+(ls*3.)*(i/2-5.);
                  y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 5.) ;
               } else {
                  padY[iPads] =-ls/2.+(ls*3.)*(i/2-4.);
                  y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.) ;
               }
            }
         }
         iPads++;
         hPad->AddBin(4,x,y);
      }
   }
   for (Int_t iCol = 0; iCol < 6; iCol ++) {
      for (Int_t i=0; i<10; i++) {
         Double_t x[4] = { r*cos(-30*deg),
                           r*cos(90*deg),
                           r*cos(210*deg),
                           r*cos(-30.*deg) };
         Double_t y[4] = { r*sin(-30*deg),
                           r*sin(90*deg),
                           r*sin(210*deg),
                           r*sin(-30.*deg) };
         for (Int_t j=0;j<4;j++) {
            padX[iPads] = (iCol+0.5)*5.+17.5;
            x[j] += padX[iPads];
            if ((iCol%2) != 0) {
               if ((i+1)%2) {
                  padY[iPads] = -l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
               } else {
                  padY[iPads] = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
               }
            } else {
               if ((i)%2) {
                  padY[iPads] = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
               } else {
                  padY[iPads] =-l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
               }
            }
         }
         iPads++;
         hPad->AddBin(4,x,y);
      }
   }
   printf("created %d pads\n",iPads);

   // create Si geometry bin
  Int_t iStrip = 0;

  nStrips = 32;
  StripX = new Double_t[32];
  StripZ = new Double_t[32];
  Double_t X[5], Z[5];
  Double_t ss = 22.65; // strip size
  for (Int_t i = 0; i < 4; i++) {
    for (Int_t j = 0; j < 8; j++) {
      if (j+8*i < 4) {
	X[0] = 122.;
	X[1] = 127.;
	X[2] = 127.;
	X[3] = 122.;
	X[4] = 122.;
	Z[0] = 97.8-j*ss;
	Z[1] = 97.8-j*ss;
	Z[2] = 97.8-(j+1)*ss;
	Z[3] = 97.8-(j+1)*ss;
	Z[4] = 97.8-j*ss;
      } else if (j+8*i < 8) {
	X[0] = 122.;
	X[1] = 127.;
	X[2] = 127.;
	X[3] = 122.;
	X[4] = 122.;
	Z[0] = -7.20-(j-4)*ss;
	Z[1] = -7.20-(j-4)*ss;
	Z[2] = -7.20-(j-4+1)*ss;
	Z[3] = -7.20-(j-4+1)*ss;
	Z[4] = -7.20-(j-4)*ss;
      } else if (j+8*i < 12) {
	X[0] = 94.150-j*ss;
	X[1] = 94.150-(j-1)*ss;
	X[2] = 94.150-(j-1)*ss;
	X[3] = 94.150-j*ss;
	X[4] = 94.150-j*ss;
	Z[0] = -169.0;
	Z[1] = -169.0;
	Z[2] = -174.0;
	Z[3] = -174.0;
	Z[4] = -169.0;
      } else if (j+8*i < 16) {
	X[0] = -48.850-(j-4)*ss;
	X[1] = -48.850-(j-4-1)*ss;
	X[2] = -48.850-(j-4-1)*ss;
	X[3] = -48.850-(j-4)*ss;
	X[4] = -48.850-(j-4)*ss;
	Z[0] = -169.0;
	Z[1] = -169.0;
	Z[2] = -174.0;
	Z[3] = -174.0;
	Z[4] = -169.0;
      } else if (j+8*i < 20) {
	X[0] = -127.0;
	X[1] = -122.0;
	X[2] = -122.0;
	X[3] = -127.0;
	X[4] = -127.0;
	Z[0] = -75.150+j*ss;
	Z[1] = -75.150+j*ss;
	Z[2] = -75.150+(j-1)*ss;
	Z[3] = -75.150+(j-1)*ss;
	Z[4] = -75.150+j*ss;
      } else if (j+8*i < 24) {
	X[0] = -127.0;
	X[1] = -122.0;
	X[2] = -122.0;
	X[3] = -127.0;
	X[4] = -127.0;
	Z[0] = 29.850+(j-4)*ss;
	Z[1] = 29.850+(j-4)*ss;
	Z[2] = 29.850+(j-4-1)*ss;
	Z[3] = 29.850+(j-4-1)*ss;
	Z[4] = 29.850+(j-4)*ss;
      } else if (j+8*i < 28) {
	X[0] = -116.80+j*ss;
	X[1] = -116.80+(j+1)*ss;
	X[2] = -116.80+(j+1)*ss;
	X[3] = -116.80+j*ss;
	X[4] = -116.80+j*ss;
	Z[0] = 174.0;
	Z[1] = 174.0;
	Z[2] = 169.0;
	Z[3] = 169.0;
	Z[4] = 174.0;
      } else if (j+8*i < 32) {
	X[0] = 26.2+(j-4)*ss;
	X[1] = 26.2+(j-4+1)*ss;
	X[2] = 26.2+(j-4+1)*ss;
	X[3] = 26.2+(j-4)*ss;
	X[4] = 26.2+(j-4)*ss;
	Z[0] = 174.0;
	Z[1] = 174.0;
	Z[2] = 169.0;
	Z[3] = 169.0;
	Z[4] = 174.0;
      }
      StripX[iStrip] = X[0] + 2.5;
      StripZ[iStrip] = Z[0] - 2.5;
      //      printf("%7.4f %7.4f %d\n",StripX[iStrip],StripZ[iStrip],iStrip);
      hPad->AddBin(5,X,Z);
      iStrip++;
    }
  }
  
  printf("created %d Strips\n", iStrip);
  return hPad;
}
