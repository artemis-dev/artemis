/**
 * @file   TCatTrackResultDisplayProcessor.cc
 * @brief  helper for displaying the result of CAT tracking
 *
 * @date   Created       : 2016-07-24 12:15:07 JST
 *         Last Modified : 2018-02-21 18:56:19 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTrackResultDisplayProcessor.h"
#include <TCatReadoutPad.h>
#include <TCatReadoutPadArray.h>
#include <TH2Poly.h>
#include <TH2D.h>
#include <TProcessor.h>
#include <TPad.h>
#include <TClonesArray.h>
#include <TCatTpcTrack.h>
#include <TCatTrackResultPulse.h>
#include <TLine.h>
#include <TGraph.h>
using art::TCatTrackResultDisplayProcessor;

ClassImp(TCatTrackResultDisplayProcessor)

TCatTrackResultDisplayProcessor::TCatTrackResultDisplayProcessor()
{
   StringVec_t definput;
   definput.push_back("track");
   fEventDisplayHelper = new TEventDisplayProcessorHelper;

   RegisterInputCollection("InputCollection","name of input collection",fNamesInput,definput);

   RegisterInputInfo("ReadoutPad","name of readout pad array",fNameReadoutPadArray,TString("readoutPad"),
                     &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
   RegisterHelper(fEventDisplayHelper);
}

TCatTrackResultDisplayProcessor::~TCatTrackResultDisplayProcessor()
{
}

TCatTrackResultDisplayProcessor::TCatTrackResultDisplayProcessor(const TCatTrackResultDisplayProcessor& rhs)
{
}

TCatTrackResultDisplayProcessor& TCatTrackResultDisplayProcessor::operator=(const TCatTrackResultDisplayProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatTrackResultDisplayProcessor::Init(TEventCollection *col)
{
   if (!fEventDisplayHelper->Displays()) return;
   if (!fNamesInput.size()) return;
   
   Int_t nInputs = fNamesInput.size();
   for (Int_t i=0; i<nInputs; ++i) {
      TEventObject* evtobj = col->Get(fNamesInput[i]);
      if (!evtobj || !evtobj->IsObject()) {
         SetStateError(Form("object '%s' is not found or is not an object",fNamesInput[i].Data()));
         return;
      }
      TObject **obj = (TObject**)evtobj->GetObjectRef();
      if (!(*obj)->InheritsFrom(TClonesArray::Class_Name()) ) {
         SetStateError(Form("object '%s' does not inherit from TClonesArray",fNamesInput[i].Data()));
         return;
      }
      TClonesArray *ca = static_cast<TClonesArray*>(*obj);
      if (!(ca)->GetClass() || !(ca)->GetClass()->InheritsFrom(TCatTpcTrack::Class_Name())) {
         SetStateError(Form("TClonesArray '%s' does not contain TCatTpcTrack",fNamesInput[i].Data()));
         return;
      }
      fInputs.push_back(reinterpret_cast<TClonesArray**>(evtobj->GetObjectRef()));
   }
   
   fHistHitsZX = (*fReadoutPadArray)->CreateHist("hitsZX","hitsZX",-160.,160.,-160.,160.);
   fHistHitsXY = new TH2D("hitsXY","hitsXY",100,-160.,160.,350,-100.,250.);
   fHistHitsYZ = new TH2D("hitsYZ","hitsYZ",350,-100.,250.,100,-160.,160.);
   fHistHitsYZ->SetMarkerStyle(20);
   fHistHitsXY->SetMarkerStyle(20);
   fLineZX = new TLine;
   fLineXY = new TLine;
   fLineYZ = new TLine;
   fEnergyCorrelation = new TH2D("histEE","energy correlation",1000,0.,1.,1000,0.,1.);
   fEnergyCorrelation->SetMarkerStyle(20);
   fEnergyCorrelation->SetXTitle("Measured (MeV)");
   fEnergyCorrelation->SetYTitle("Calculated (MeV)");
   TPad *pad = fEventDisplayHelper->GetMainPad();   
   pad->Divide(2,2);
   fEventDisplayHelper->Process();
}


void TCatTrackResultDisplayProcessor::Process()
{
   if (!fEventDisplayHelper->Displays()) return;
   fHistHitsZX->ClearBinContents();
   fHistHitsXY->Reset();
   fHistHitsYZ->Reset();
   fEnergyCorrelation->Reset();
   //////////////////////////////////////////////////
   // prepare histograms to be drawn
   //////////////////////////////////////////////////
   Double_t emax = 0.;
   Double_t cmax = 0.;
   const Int_t nInputs = fInputs.size();
   
   for (Int_t iIn = 0; iIn < nInputs; ++iIn) {
      TClonesArray *input = *fInputs[iIn];
      const Int_t nRes = input->GetEntriesFast();
      for (Int_t iRes = 0; iRes < nRes; ++iRes) {
         TCatTpcTrack *result = static_cast<TCatTpcTrack*>(input->UncheckedAt(iRes));
         const Int_t nHits = result->GetHits().size();
         Double_t diff2 = 0.;
         for (Int_t iHits = 0; iHits < nHits; ++iHits) {
            TCatTrackResultPulse *pulse = static_cast<TCatTrackResultPulse*>(result->GetHits().at(iHits));
            Double_t e = pulse->GetEnergyDeposit(); // estimated energy deposit
            Double_t c = pulse->GetCharge(); // measured energy deposit
            fHistHitsZX->Fill(pulse->GetX(),pulse->GetZ(),c);
            fHistHitsXY->Fill(pulse->GetX(),pulse->GetY());
            fHistHitsYZ->Fill(pulse->GetY(),pulse->GetZ());
            if (emax < e) {
               emax = e+0.1;
            }
            if (cmax < c) {
               cmax = c+0.1;
            }
            fEnergyCorrelation->Fill(pulse->GetCharge(),pulse->GetEnergyDeposit());
            e /= 37e-6;
            c /= 37e-6;
            if (fVerboseLevel > 1) {
               printf("id = %3d, c = %10.5f, e = %10.5f, y = %10.5f, diff = %10.5f\n",pulse->GetID(),pulse->GetCharge(),pulse->GetEnergyDeposit(),pulse->GetY(), c-e);
            }
            diff2 = (e-c)*(e-c)/c;
         }
         if (fVerboseLevel > 1) {
            printf("diff2 = %10.5f\n",diff2);
         }
         fLineZX->SetX1(result->GetStartPoint().X());
         fLineZX->SetY1(result->GetStartPoint().Z());
         fLineZX->SetX2(result->GetEndPoint().X());
         fLineZX->SetY2(result->GetEndPoint().Z());
         fLineXY->SetX1(result->GetStartPoint().X());
         fLineXY->SetY1(result->GetStartPoint().Y());
         fLineXY->SetX2(result->GetEndPoint().X());
         fLineXY->SetY2(result->GetEndPoint().Y());

         fLineYZ->SetX1(result->GetStartPoint().Y());
         fLineYZ->SetY1(result->GetStartPoint().Z());
         fLineYZ->SetX2(result->GetEndPoint().Y());
         fLineYZ->SetY2(result->GetEndPoint().Z());
      }
   }
           
   
   TPad *pad = fEventDisplayHelper->GetMainPad();
   pad->cd(1);
   fHistHitsZX->Draw("colz");
   fLineZX->Draw();
   pad->cd(2);
   fHistHitsYZ->Draw("");
   fLineYZ->Draw();
   pad->cd(3);
   fHistHitsXY->Draw("");
   fLineXY->Draw();
   pad->cd(4);
   fEnergyCorrelation->GetXaxis()->SetRangeUser(0.,TMath::Max(emax,cmax));
   fEnergyCorrelation->GetYaxis()->SetRangeUser(0.,TMath::Max(emax,cmax));
   fEnergyCorrelation->Draw("");
   fEventDisplayHelper->Process();
}
