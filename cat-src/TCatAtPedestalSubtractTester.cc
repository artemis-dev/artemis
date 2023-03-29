/* $Id:$ */
/**
 * @file   TCatAtPedestalSubtractTester.cc
 * @date   Created : 2015-03-12 21:07:34 JST
 *   Last Modified : 2017-12-23 21:20:05 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TCatAtPedestalSubtractProcessor.cc
 *  
 *    Copyright (C)2015
 */
#include "TCatAtPedestalSubtractTester.h"
#include <TCatPulseShape.h>
#include <TMath.h>

#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>

#include <TStyle.h>
#include <sstream>
#ifndef __CINT__
#include <iomanip>
#endif
art::TCatAtPedestalSubtractTester::TCatAtPedestalSubtractTester()
   : fEvt(0), fRunHeaders(NULL), fRunInfo(NULL), fRunName(""), fRunNumber(0), 
     fCutSigma(kFALSE), fhPedMean(NULL), fhPedSigma(NULL)
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","fadc rawdata (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatPulseShape)",
                            fOutputColName,TString("atrawsub"));
   RegisterOutputCollection("OutputSigmaCollection","output sigma-collection name",
                            fOutputSigmaColName,TString("sigma"));
   RegisterProcessorParameter("LowSigma","Low Sigma to draw",fLowSigma,(Float_t)-1E9);
   RegisterProcessorParameter("HighSigma","High Sigma to draw",fHighSigma,(Float_t)1E9);
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("pedsubtester.log"));
   RegisterProcessorParameter("HistSaveFlag","Flag For Histgram Save",fSaveHist,false);
   RegisterProcessorParameter("DrawFigFlag","Flag of Draw Figure",fDrawFig,false);
   RegisterProcessorParameter("CanvasX","X Position of Canvas",fCanvasX,(Int_t)0);
   RegisterProcessorParameter("CanvasY","Y Position of Canvas",fCanvasY,(Int_t)0);
   RegisterProcessorParameter("CanvasW","Width of Canvas",fCanvasW,(Int_t)1000);
   RegisterProcessorParameter("CanvasH","Hight of Canvas",fCanvasH,(Int_t)1050);
   RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
   RegisterProcessorParameter("SaveFigFlag","Flag of Save Figure",fSaveFig,false);
   RegisterProcessorParameter("FigFilePath","Path of Figure File",fFigFilePath,TString("./"));
   RegisterProcessorParameter("AddFigFilePath","Add Path of Figure File",fAddFigFilePath,TString("./"));
   RegisterProcessorParameter("SaveEvtNum","Number of Save Event",fSaveEvt,(Int_t)-1);
   RegisterProcessorParameter("SaveHitNum","Number of Save Hit",fSaveHit,(Int_t)-1);
   RegisterProcessorParameter("ViewID","Number of View ID",fViewID,(Int_t)-1);
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
   RegisterOptionalParameter("OutputInvalid","flag to output invalid data if non zero  (default : 0) ",fOutputInvalid,kFALSE);
   RegisterOptionalParameter("CutSigma","flag to cut sigma more than 5 if non zero  (default : 0) ",fCutSigma,kFALSE);
}
art::TCatAtPedestalSubtractTester::~TCatAtPedestalSubtractTester()
{
   if (fCATHitOut) delete fCATHitOut;   
   if (fSigma) delete fSigma;   
   if (fSaveHist) {
     if (fhPedMean) delete fhPedMean;
     if (fhPedSigma) delete fhPedSigma;
     if (fh1DPedSigma) delete fh1DPedSigma;
     if (fhNSamples) delete fhNSamples;
     if (fhNSamplesSigma) delete fhNSamplesSigma;
   }
   if (fDrawFig) if (fPulseDrawer) delete fPulseDrawer;
}

void art::TCatAtPedestalSubtractTester::Init(art::TEventCollection *col)
{
   if (col->Get(fInputColName.front())) {
      fCATHit = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
   }
   Info("Init","TCatAtPedestalSubtractTester: Input = %s",fInputColName.front().Data());
   if (!fCATHit) {
      Error("Init","Collection '%s' does not exist",fInputColName.front().Data());
      return;
   }

   fCATHitOut = new TClonesArray("art::TCatPulseShape");
   if (!fCATHitOut) {
      Error("Init","Faild to initialize output collection");
      return;
   }
   fCATHitOut->Clear("C");
   fCATHitOut->SetName(fOutputColName);
   col->Add(fOutputColName,fCATHitOut,fOutputIsTransparent);

   //fSigma = new TClonesArray();
   //if (!fSigma) {
   //   Error("Init","Faild to initialize output sigma-collection");
   //   return;
   //}
   //fSigma->Clear("C");
   //col->Add(fOutputSigmaColName,fSigma,fOutputIsTransparent);

   // Prepare Log/Fig File
   if (fDebug || fDetailDebug) {
      const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
      fp = fopen(LogNamePath,"w");
      if (fp == NULL) {
         Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
         exit(1);
      }
   }
   
   // Prepare Figure and Histgram
   if (fDrawFig) {
      fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fOutputColName.Data(),fAddFigFilePath.Data());
      fPulseDrawer = new TPulseDrawer(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
      fPulseDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFile);
      fPulseDrawer->SetViewHit(fSaveHit);
      fPulseDrawer->SetRunHeader(fRunHeaders);
   }

   if (fSaveHist) {
     gStyle->SetOptStat(1);
     fhPedMean = new TH2F("hPedMean","hPedMean;Pad ID;Mean",400,0.5,400.5,300,2000.,2300.);
     fhPedSigma = new TH2F("hPedSigma","hPedSigma;Pad ID;Sigma",400,0.5,400.5,200,0.,50.);
     fh1DPedSigma = new TH1F("h1DPedSigma","h1DPedSigma",200,0.,50.);
     fhNSamples = new TH1F("hNSamples","hNSamples",500,0.,500.);
     fhNSamplesSigma = new TH2F("hNSamplesSigma","hNSamplesSigma;# of samples;Sigma",500,0.,500.,200,0.,50.);
   }
}


void art::TCatAtPedestalSubtractTester::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }

   fCATHitOut->Clear("C");
   //fSigma->Clear("C");
   Int_t nHit = (*fCATHit)->GetEntriesFast();
   if (fDebug) if (nHit) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHit = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHit);
   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)((*fCATHit)->At(iHit));
      TCatPulseShape *dest = (TCatPulseShape*)fCATHitOut->ConstructedAt(fCATHitOut->GetEntriesFast());
      //Double_t *sigma = (Double_t*)fSigma->ConstructedAt(fSigma->GetEntriesFast());
      std::vector<Float_t>  &inSample  = hit->GetSample();
      std::vector<Float_t>  &outSample = dest->GetSample();
      std::vector<Float_t>  &outClock  = dest->GetClock();
      hit->Copy(*dest);
      Double_t ped = std::accumulate(inSample.begin(),inSample.begin()+10,0.);
      Double_t ped2 = std::inner_product(inSample.begin(),inSample.begin()+10,inSample.begin(),0.);
      ped /= 10.;
      ped2/=10.;
      ped2 -= ped*ped;
      ped2 = TMath::Sqrt(ped2);

      if (hit->GetID()>=400) {
         if (fDebug) fprintf(fp,"iHit = %3d, id = %3d, ID >= 400\n",iHit,hit->GetID());
         continue; 
      }
      if (fDetailDebug) fprintf(fp,"iHit = %3d, ped = %5.1f, sigma = %5.1f\n",iHit,ped,ped2);
      //=========================================================================

      // Save
      int num = inSample.size();
      if (fDebug) fprintf(fp,"iHit = %3d, num = %3d\n",iHit,num);
#if fCutSigma
      if (ped2>=5.) {
         dest->TestQualityBit(TCatPulseShape::kInvalid);
         //dest->Invalidate();
         if (fDebug) fprintf(fp,"Invalid: Evt = %d, iHit = %d, sigma = %0.1f\n",fEvt,iHit,ped2);
         if (fDetailDebug) printf("[subtracter] Invalid: Evt = %d, iHit = %d, sigma = %0.1f\n",fEvt,iHit,ped2);
         if (!fOutputInvalid) continue; 
      } else {
         dest->Validate();
      }
#else
      dest->Validate();
#endif
#if 0 // for Check
      if (ped2<5.) {
         dest->TestQualityBit(TCatPulseShape::kInvalid);
         //dest->Invalidate();
         if (fDebug) fprintf(fp,"Invalid: Evt = %d, iHit = %d, sigma = %0.1f\n",fEvt,iHit,ped2);
         if (fDetailDebug) printf("[subtracter] Invalid: Evt = %d, iHit = %d, sigma = %0.1f\n",fEvt,iHit,ped2);
         if (!fOutputInvalid) break;
      } else {
         dest->Validate();
      }
#endif

      if (ped2<=fLowSigma) continue;
      if (fViewID!=-1) if (hit->GetID()!=fViewID) continue;
      if (ped2>=fHighSigma) continue;
      outSample.resize(inSample.size());
      std::transform(inSample.begin(),inSample.end(),outSample.begin(),
                     std::bind(std::minus<float>(),std::placeholders::_1, ped));
      outClock.resize(num);
      dest->SetNumSample(num);
      for (Int_t i=0; i!=num; i++) {
         outClock[i] = i;
         if (fDetailDebug) fprintf(fp,"i = %3d, outClock[%3d] = %3.0f, inSample[%3d] = %6.1f\n",i,i,outClock[i],i,inSample[i]);
      }

      if (fDrawFig) {
         //printf("Evt = %d, iHit = %3d, id = %3d, ped = %5.1f, sigma = %5.1f\n",fEvt,iHit,hit->GetID(),ped,ped2);
         if (fDetailDebug) if (num==0) printf("Evt = %d, iHit = %d, num = 0 !!\n",fEvt,iHit);
         TObject *hitobj = hit;
         fPulseDrawer->SetPulse(*hitobj);
         fPulseDrawer->SetGraphRange(0.,100.,2050.,2300.);
         fPulseDrawer->SetEvent(fEvt,iHit);
         //fPulseDrawer->SetTF1(0,ped);
         fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f",fEvt,hit->GetID(),hit->GetOffset()),1);
         fPulseDrawer->SetTextNDC(Form("nHit = %3d, iHit = %3d, nSample = %3d",nHit,iHit,num),5,0.10,0.91);
         fPulseDrawer->SetTextNDC(Form("ped = %6.1f, sigma = %4.1f",ped,ped2),2);
         fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.005);
         fPulseDrawer->Draw();
      }
      if (fSaveHist) {
	fhNSamples->Fill(num);
	fhNSamplesSigma->Fill(num,ped2);
        fhPedSigma->Fill(hit->GetID(),ped2);
	fhPedMean->Fill(hit->GetID(),ped);
	fh1DPedSigma->Fill(ped2);
      }
      //=========================================================================
   }
   fEvt++;
}
