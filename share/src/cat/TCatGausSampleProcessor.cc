/* $Id:$ */
/**
 * @file   TCatGausSampleProcessor.cc
 * @date   Created : Mar 11, 2013 05:11:49 JST
 *   Last Modified : Jan 08, 2014 16:04:10 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatGausSampleProcessor.h"
#include <TCategorizedData.h>
#include "TCatPulseShape.h"
#include <TStopwatch.h>
#include <TMath.h>

vector<double> art::TCatGausSampleProcessor::FCN::fX;
vector<double> art::TCatGausSampleProcessor::FCN::fY;
Int_t art::TCatGausSampleProcessor::FCN::fN;
double art::TCatGausSampleProcessor::FCN::fFCN;

ClassImp(art::TCatGausSampleProcessor)

art::TCatGausSampleProcessor::TCatGausSampleProcessor()
   : fCATHit(NULL), fCATHitOut(NULL), fNPar(3), fMinuit(3)
{
   StringVec_t defInput;
   defInput.push_back("atrawsub");
   RegisterInputCollection("InputCollection","pedestal subtracted pulse (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatRadcRawData)",
                            fOutputColName,TString("atpulse"));
   RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwize hit will be added ordinaly (default: 1)",fIsSparse,1);
   RegisterProcessorParameter("Sigma","fixed sigma for fitting. if set value is negative, the parameter is not fixed",fSigma,(Float_t)1.);
}
art::TCatGausSampleProcessor::~TCatGausSampleProcessor()
{
   if (fCATHitOut && fCATHitOut != fCATHit) {
      delete fCATHitOut;
      fCATHitOut = NULL;
   }
   if (fHistFitterFit) delete fHistFitterFit;
}
void art::TCatGausSampleProcessor::Init(TEventCollection *col)
{
  fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
  if (!fCategorized) {
    printf("Initialization failed with %s\n",fInputColName.front().Data());
  }
  fCATHit = new TClonesArray("TCatFadcRawData");
  if (!fCATHitOut) {
    fCATHitOut = new TClonesArray("TCatPulseShape");
    fCATHitOut->SetName(fOutputColName);
    col->Add(fOutputColName,fCATHit,fOutputIsTransparent);
  }
  fHistFitterFit = new TH1F("hFitterFit","Fitter Fit Time",100,0.,1000.);
  fHistMinuitFit = new TH1F("hMinuitFit","Minuit Fit Time",100,0.,1000.);
  fHistMean = new TH2F("hGausMean","Mean",400,0.,400.,100,0.,100.);
  fHistSigma = new TH2F("hGausSigma","sigma",400,0.,400.,100,0.,20);
  fHistConst = new TH2F("hGausConst","const",400,0.,400.,100,0.,1000.);
//  int ierflg = 0; // unused
  fMinuit.SetFCN(FCN::calcfcn);
  fMinuit.SetPrintLevel(-1);
}

void art::TCatGausSampleProcessor::Process()
{
   if (fCATHitOut != fCATHit) {
      // copy 
      TIter next(fCATHit);
      TObject *obj = NULL;
      fCATHitOut->Clear("C");
      while ((obj = next())) {
         TCatPulseShape *hit  = (TCatPulseShape*)obj;
         TCatPulseShape *dest;
         if (fIsSparse) {
            dest = (TCatPulseShape*)fCATHitOut->ConstructedAt(fCATHitOut->GetEntriesFast());
         } else {
            dest = (TCatPulseShape*)fCATHitOut->ConstructedAt(hit->GetID());
         }
         Double_t max = 0;
         Double_t maxbin = 0;
         Int_t nSample = hit->GetNumSample();
         TStopwatch stopwatch;
         dest->SetID(hit->GetID());
         dest->SetTime(hit->GetTime());
         dest->SetGeo(hit->GetGeo());
         dest->SetCh(hit->GetCh());
         dest->SetOffset(hit->GetOffset());
         fFCN.Clear();
         for (Int_t i=0; i!=nSample; i++) {
            if (max < (*hit)[i]) {
               max = (*hit)[i];
               maxbin = (*hit)(i);
            }
            dest->AddSample((Float_t)((*hit)[i]),(*hit)(i));
         }
         Int_t first = TMath::CeilNint((maxbin-20.)>0?(maxbin-20):0);
         Int_t end = TMath::FloorNint((maxbin+15)<nSample?(maxbin+15):nSample);
         for (Int_t i=first; i<end;i++) {
            fFCN.Add((*hit)(i),(*hit)[i]);
         }
         stopwatch.Start();
         int ierflg = 0;
         fMinuit.mnparm(0,"Const",max,1,-10000.,10000.,ierflg);
         fMinuit.mnparm(1,"Mean", maxbin,1,-10000.,10000.,ierflg);
         fMinuit.mnparm(2,"Sigma",abs(fSigma),0.01,0.,1000.,ierflg);
         if (fSigma < 0) {
            fMinuit.FixParameter(2);
         }
         fMinuit.Migrad();
         stopwatch.Stop();
         fHistMinuitFit->Fill(stopwatch.RealTime()*1e6);
         double dummy, charge, time,sigma;
         fMinuit.GetParameter(0,charge,dummy);
         fMinuit.GetParameter(1,time,dummy);
         fMinuit.GetParameter(2,sigma,dummy);
         fHistConst->Fill(hit->GetID(),charge);
         fHistMean->Fill(hit->GetID(),time);
         fHistSigma->Fill(hit->GetID(),sigma);
         dest->SetCharge(charge);
         dest->SetTime(hit->GetTime() + time);
         if (charge > 0 && charge < 4000.) {
            dest->ResetQualityBit(TCatPulseShape::kInvalid);
         }
      }
      if (!fIsSparse) {
         Int_t n = fCATHitOut->GetEntriesFast();
         for (Int_t i=0; i!=n; i++) {
            fCATHitOut->ConstructedAt(i);
         }
      }
   } else {
      TIter next(fCATHit);
      TObject *obj;
      while ((obj = next())) {
         TCatPulseShape *hit  = (TCatPulseShape*)obj;
         Int_t nSample = hit->GetNumSample();
         Double_t charge = 0.;
         for (Int_t i=0; i!=nSample; i++) {
            charge += (*hit)[i];
         }
         hit->SetCharge(charge);
      }
   }
}
