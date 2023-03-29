/* $Id:$ */
/**
 * @file   TCatAtHitPatternProcessor.cc
 * @date   Created : Feb 10, 2013 10:10:17 JST
 *   Last Modified : 2018-02-24 20:14:16 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtHitPatternProcessor.h"
#include <TCategorizedData.h>
#include <TH2Poly.h>
#include <TMath.h>
#include <TROOT.h>
#include <TCanvas.h>
#include "TCatFadcRawData.h"
#include <TROOT.h>
#include <unistd.h>
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TEventHeader.h>
#include <TText.h>

ClassImp(art::TCatAtHitPatternProcessor)

art::TCatAtHitPatternProcessor::TCatAtHitPatternProcessor()
{
   fPads = NULL;
   fCanvas = NULL;
   fText = NULL;
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","flash adc raw data",
                           fInputColName,defInput);
   RegisterInputCollection("EventHeader","event header",fEventHeaderName,"eventheader",
                           &fEventHeader,art::TEventHeader::Class_Name());
                           
   RegisterProcessorParameter("HistName","HistName",fHistName,TString("hHitPattern"));
   RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
   RegisterProcessorParameter("WidthDX","half width of x axis",fWdX,50.);
   RegisterProcessorParameter("WidthDY","half width of y axis",fWdY,50.);
   RegisterInputInfo("ReadoutPad","Name of Readout Pad",fReadoutPadName,
                     TString("readoutPad"),&fPads,"art::TCatReadoutPadArray","");
   
   
}
art::TCatAtHitPatternProcessor::~TCatAtHitPatternProcessor()
{
   if (fText) delete fText;
   if (fCanvas) delete fCanvas;

}

void art::TCatAtHitPatternProcessor::Init(art::TEventCollection *col) {
   fAnalyzed = 0;
   if (col->Get(fInputColName.front())) {
      fFADC = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
   } else {
      printf("<TCatAtHitPatternProcessor> input collection (%s) does not exist\n",fInputColName.front().Data());
   }
   gDirectory = gROOT;
   fHPad = (*fPads)->CreateHist("hPad","pad",-fWdX,fWdX,-fWdY,fWdY);
   fHPadEvt = (*fPads)->CreateHist("hPad2","pad",-fWdX,fWdX,-fWdY,fWdY);
   gDirectory->Add(fHPad);
   gDirectory->Add(fHPadEvt);
//   col->Add(fHPad,fOutputIsTransparent);
//   fCanvas = (TCanvas*)gROOT->FindObjectAny("c1");
//   gDirectory->Add(fHPad);
//   gDirectory->Add(fHPadEvt);
   if (fWaitTime>0) {
      fCanvas = new TCanvas(GetName(),GetName());
   }
   fText  = new TText;
}

void art::TCatAtHitPatternProcessor::Process()
{
   fAnalyzed++;
   if (!fFADC) return;
   Int_t nHit = (*fFADC)->GetEntries();
//   fHPad->ClearBinContents();
   fHPadEvt->ClearBinContents();
   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TCatFadcRawData* d = (TCatFadcRawData*)(**fFADC)[iHit];
      Int_t id = d->GetID();
      if (id < (*fPads)->GetEntriesFast()) {
         TCatReadoutPad *pad = (TCatReadoutPad*)(*fPads)->At(id);
#if 0      
         fHPad->Fill(fPadX[id],fPadY[id]);
         fHPadEvt->Fill(fPadX[id],fPadY[id]);
#else      
         fHPad->Fill(pad->X(),pad->Y());
         fHPadEvt->Fill(pad->X(),pad->Y());
#endif
      }
   }
   if ( fWaitTime>0) {
      fCanvas->cd();
      fHPadEvt->Draw("colz");
      fText->SetText(0.1,0.9,Form("Total Event# = %d (Accepted Event# = %d)",(*fEventHeader)->GetEventNumber(),fAnalyzed));
      fText->SetNDC();
      fText->Draw();
      fCanvas->Update();
      fCanvas->Modified();
      usleep(fWaitTime);
   }
//   sleep(1);
}


TH2Poly* art::TCatAtHitPatternProcessor::DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY) 
{
   const double deg = TMath::Pi()/180.;
   Double_t l = 10. * sqrt(3)/2. * 2/3.;
   Double_t r =  9. * sqrt(3)/2. * 2/3.;
   Double_t ls = 5. * sqrt(3)/2. * 2/3.;
   Double_t rs = 4. * sqrt(3)/2. * 2/3.;
   Int_t iPads = 0;
   
   nPads = 400;
   padX = new Double_t[400];
   padY = new Double_t[400];
   TH2Poly *hPad = new TH2Poly("hPad","hPad",-50.,50.,-50.,50.);
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
   return hPad;
}
