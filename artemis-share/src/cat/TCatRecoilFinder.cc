/* $Id:$ */
/**
 * @file   TCatRecoilFinder.cc
 * @date   Created : Nov 01, 2013 22:01:43 JST
 *   Last Modified : 2014-05-15 15:06:46 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatRecoilFinder.h"

#include <TH2Poly.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TMath.h>
#include <unistd.h>

using art::TCatRecoilFinder;

TCatRecoilFinder::TCatRecoilFinder()
{
   StringVec_t defInput; defInput.push_back("atsum");
   RegisterInputCollection("InputCollection","Pulse shape with charge (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
}
TCatRecoilFinder::~TCatRecoilFinder()
{
}

void TCatRecoilFinder::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      printf("[TCatSumSampleProcessor]: No such input %s\n",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   printf("<TCatRecoilFinder> Input = %s\n",fInputColName.front().Data());
   fHPad       = DrawPad(fNPad,fPadX,fPadY);
   fHPadTime   = (TH2Poly*)fHPad->Clone("hPadCharge");
   fHPadCharge = (TH2Poly*)fHPad->Clone("hPadTime");
   fHPadTime->SetTitle("Time");
   fHPadCharge->SetTitle("Charge");
   fCanvas = new TCanvas("cHits","Recoile Candidate");
   fCanvas->Divide(2,2);
   fHCharge = new TH2F("hCharge","hCharge",800,-400.,400,200,0.,200.);
   fNumEvt = 0;
}


void TCatRecoilFinder::Process()
{
   Int_t nHit = (*fInput)->GetEntries();
   Bool_t isHit = kFALSE;
   Bool_t isUsed[10000];
   fHPad->ClearBinContents();
   fHPadTime->ClearBinContents();
   fHPadCharge->ClearBinContents();
   fHCharge->Reset();
   (*fInput)->Sort();
   for (Int_t iHit1 =0; iHit1 != nHit; iHit1++) {
      isUsed[iHit1] = kFALSE;
   }
   TCatPulseShape *pulseSaved = NULL;
   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*)((**fInput)[iHit]);
      const Int_t &id = pulse->GetID();
      const Double_t &charge = pulse->GetCharge();
      const Double_t &offset = pulse->GetOffset();
      for (Int_t iHit2 = 0; iHit2 != nHit; iHit2++) {
         if (iHit2 == iHit) continue;
         TCatPulseShape *pulse2 = (TCatPulseShape*)((**fInput)[iHit2]);
         const Int_t &id2 = pulse2->GetID();
         const Double_t &charge2 = pulse2->GetCharge();
         const Double_t &offset2 = pulse2->GetOffset();
         if (TMath::Abs(id-id2) != 10 && TMath::Abs(id-id2) != 20) continue;
            
         if (TMath::Abs(offset - offset2) < 20 && 
             (1 || charge > 1500) || (charge2 > 1500)) {
            if (!isUsed[iHit]) {
               fHPad->Fill(fPadX[id],fPadY[id]);
               fHPadTime->Fill(fPadX[id],fPadY[id],offset);
               fHPadCharge->Fill(fPadX[id],fPadY[id],charge);
               Int_t ns = pulse->GetNumSample();
               for (Int_t is=0; is!=ns; is++) {
                  fHCharge->Fill(is+offset,(*pulse)[is]);
               }
               isUsed[iHit] = kTRUE;
            }
            if (!isUsed[iHit2]) {
               fHPad->Fill(fPadX[id2],fPadY[id2]);
               fHPadTime->Fill(fPadX[id],fPadY[id],offset2);
               fHPadCharge->Fill(fPadX[id],fPadY[id],charge2);
               Int_t ns = pulse2->GetNumSample();
               for (Int_t is=0; is!=ns; is++) {
                  fHCharge->Fill(is+offset2,(*pulse2)[is]);
               }
               isUsed[iHit2] = kTRUE;
            }
         }
      }
   }
   fNumEvt++;
   if (fWaitTime > 0) {
      fCanvas->Clear();
      fCanvas->Divide(2,2);
      fCanvas->cd(1);
      fHPad->Draw("colz");
      fCanvas->cd(2);
      fHPadTime->Draw("colz");
      fCanvas->cd(3);
      fHPadCharge->Draw("colz");
      fCanvas->cd(4);
      fHCharge->Draw("colz");
      fCanvas->Update();
      fCanvas->Modified();
      usleep(fWaitTime);
   }
}

void TCatRecoilFinder::PostLoop() 
{
   printf("Evt = %d\n",fNumEvt);
   fCanvas->Clear();
   fCanvas->Divide(2,2);
   fCanvas->cd(1);
   fHPad->Draw("colz");
   fCanvas->cd(2);
   fHPadTime->Draw("colz");
   fCanvas->cd(3);
   fHPadCharge->Draw("colz");
   fCanvas->cd(4);
   fHCharge->Draw("colz");
   fCanvas->Update();
   fCanvas->Modified();
}



TH2Poly* TCatRecoilFinder::DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY) 
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
