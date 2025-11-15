/* $Id:$ */
/**
 * @file   TPulseFindTester.cc
 * @date   Created : 2015-03-29 17:44:58 JST
 *   Last Modified : 2016-08-31 21:40:17 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2015
 */
#include "TPulseFindTester.h"
#include <TStyle.h>
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <TMath.h>

ClassImp(art::TPulseFindTester);

using art::TPulseFindTester;

TPulseFindTester::TPulseFindTester()
   : fEvt(0), fNChargeZeroHits(0), fRunHeaders(NULL), fRunInfo(NULL), fRunName(""), fRunNumber(0)
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName, def);
   RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
                            fOutputColName,TString("atpulse"));
   RegisterProcessorParameter("RisingThreshold","threshold for rising edge",
                              fRisingThreshold,(Float_t)10.);
   RegisterProcessorParameter("FallingThreshold","threshold for falling edge",
                              fFallingThreshold,(Float_t)10.);
   RegisterProcessorParameter("SavePulse","flag to save pulse",
                              fSavePulse,kFALSE);
   RegisterProcessorParameter("MaxLength","maximum length to be valid",
                              fMaxLength,150);
   RegisterProcessorParameter("PrePulse","the number of sampling for pre-pulse",
                              fPrePulse,10);
   RegisterProcessorParameter("PostPulse","the number of sampling for pre-pulse",
                              fPostPulse,10);
   RegisterProcessorParameter("Sparse","sparse mode (first hist is stored)",
                              fIsSparse,kFALSE);
   FloatVec_t range;
   range.push_back(-500.);
   range.push_back(500.);
   RegisterProcessorParameter("OffsetRange","minimum and maximum range of offset",
                              fOffsetRange,range);
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("pulsefindtester.log"));
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
   RegisterProcessorParameter("SaveIDNum","Number of Save ID",fSaveID,(Int_t)-1);
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
   RegisterProcessorParameter("CutNegativeCharge","Flag For Cutting Negative Charge",fCutNegativeCharge,(Bool_t)kFALSE);
   RegisterProcessorParameter("CutLowCharge","Set Low Charge for Cutting",fCutLowCharge,(Float_t)-1E9);
   RegisterProcessorParameter("CutHighCharge","Set High Charge for Cutting",fCutHighCharge,(Float_t)+1E9);
   RegisterProcessorParameter("CutLowOffset","Set Low Offset for Cutting",fCutLowOffset,(Float_t)-1E9);
   RegisterProcessorParameter("CutHighOffset","Set High Offset for Cutting",fCutHighOffset,(Float_t)+1E9);
   RegisterProcessorParameter("GraphXmin","Set X Axis min Range",fGraphXmin,(Float_t)  0.);
   RegisterProcessorParameter("GraphXmax","Set X Axis max Range",fGraphXmax,(Float_t)100.);
   RegisterProcessorParameter("GraphYmin","Set Y Axis min Range",fGraphYmin,(Float_t)-10.);
   RegisterProcessorParameter("GraphYmax","Set Y Axis max Range",fGraphYmax,(Float_t)100.);
   StringVec_t defSigma;
   def.push_back("sigma");
   RegisterInputCollection("InputSigmaCollection","sigma data (TClonesArray)",
                           fSigmaColName,defSigma);
}
TPulseFindTester::~TPulseFindTester()
{
   if (fDebug || fDetailDebug) fclose(fp);
   if (fDrawFig) {
      if (fPulseDrawer) delete fPulseDrawer;
      if (fhGraph2) delete fhGraph2;
   }
   if (fSaveHist) {
      if (fhYAbsYAbsY)  delete fhYAbsYAbsY;
      if (fhSigmaYAbsY) delete fhSigmaYAbsY;
      if (fhSigmaYAbsY2) delete fhSigmaYAbsY2;
      if (fhCharge) delete fhCharge;
      if (fhOffset) delete fhOffset;
      if (fhOffsetCharge) delete fhOffsetCharge;      
      if (fhSigmaCharge)  delete fhSigmaCharge;
   }
}


void TPulseFindTester::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   fOutput = new TClonesArray("art::TCatPulseShape");
   col->Add(fOutputColName,fOutput,fOutputIsTransparent);
   // Prepare Log File
   if (fDebug || fDetailDebug) {
      const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
      fp = fopen(LogNamePath,"w");
      if (fp == NULL) {
         Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
         exit(1);
      }
   }
   Info("Init","RisingThreshold = %0.1f, FallingThreshold = %0.1f",fRisingThreshold,fFallingThreshold);

   //if (!(obj = col->Get(fSigmaColName.front()))) {
   //   Info("Init","No such input sigma %s",fSigmaColName.front().Data());
   //   return;
   //}
   //fSigma = (TClonesArray**) obj->GetObjectRef();

   // Prepare Figure and Histgram
   fhGraph2 = new TGraph;
   if (fDrawFig) {
      fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fOutputColName.Data(),fAddFigFilePath.Data());
      fPulseDrawer = new TPulseDrawer(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
      fPulseDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFile);
      fPulseDrawer->SetViewHit(fSaveHit);
      fPulseDrawer->SetRunHeader(fRunHeaders);
   }
   if (fSaveHist) {
      gStyle->SetOptStat(11);
      fhYAbsYAbsY  = new TH2F("hYAbsYAbsY","hYAbsYAbsY;sum(YAbs);sum(YAbs-Y)",100,0,1000,100,0,1000);
      fhSigmaYAbsY = new TH2F("hSigmaYAbsY","hSigmaYAbsY;Sigma;sum(YAbs-Y)",50,0.,50.,100,0,1000);
      fhSigmaYAbsY2 = new TH2F("hSigmaYAbsY2","hSigmaYAbsY Non Zero;Sigma;sum(YAbs-Y)",100,0.5,10.5,100,0.5,100.5);
      fhCharge = new TH1F("hCharge","hCharge",510,-100.,5000.);
      fhOffset = new TH1F("hOffset","hOffset",100,0.,100.);
      fhOffsetCharge = new TH2F("hOffsetCharge","hOffsetCharge;Offset;Charge",100,0.,100.,110,-100.,1000.);
      fhSigmaCharge  = new TH2F("hSigmaCharge","hSigmaCharge;Sigma;Charge",70,0.,70.,150,-5000.,10000.);
   }
}


void TPulseFindTester::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   fOutput->Clear("C");
   Int_t nHits = (*fInput)->GetEntriesFast();
   //Int_t nSigmas = (*fSigma)->GetEntriesFast();
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   //if (nHits) printf("Evt = %d, nHits = %3d, nSigmas = %d\n",fEvt,nHits,nSigmas);
   if (fDetailDebug && nHits) printf("Evt = %d, nHits = %3d\n",fEvt,nHits);
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (*fInput)->At(iHit);
      if (fDebug) fprintf(fp,"Evt = %d, iHit = %d, id = %d\n",fEvt,iHit,hit->GetID());
#if 0
      if (!(hit->IsValid())) {
         if (fDebug) fprintf(fp,"Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,hit->GetID());
         if (fDetailDebug) printf("[pulsefind] Evt = %d, iHit = %d, id = %d, Invalid pulse\n",fEvt,iHit,hit->GetID());
         continue;
      }
#endif
      //Double_t *sigma = (Double_t*) (*fSigma)->At(iHit);
      const std::vector<Float_t>& sample = hit->GetSample();
      const std::vector<Float_t>::const_iterator itbegin = sample.begin();
      const std::vector<Float_t>::const_iterator itend = sample.end();
      std::vector<Float_t>::const_iterator it = sample.begin();
      std::vector<Float_t>::const_iterator itfirst;
      std::vector<Float_t>::const_iterator itlast;
      Double_t q = -1E9;
      while (it != itend) {
         //=========================================================================

         Float_t rttiming = -1E9;
         Float_t offset   = -1E9;
         //Double_t ped = std::accumulate(sample.begin(),sample.begin()+10,0.);
         Double_t ped = 0.;
         Double_t ped2 = std::inner_product(sample.begin(),sample.begin()+10,sample.begin(),0.);
         ped /= 10.;
         ped2/=10.;
         ped2 -= ped*ped;
         ped2 = TMath::Sqrt(ped2);
         //=========================================================================

         itfirst = std::find_if(it,itend,std::bind(std::greater<Float_t>(),std::placeholders::_1,fRisingThreshold));
         it = itfirst;
         if (itfirst != itend) {
            // pulse found, search end pont
            itlast = (itfirst+fPostPulse > itend)?itend:itfirst+fPostPulse;
            // @todo remove magic number 100. (typical pulse length?)
            std::vector<Float_t>::const_iterator peak_it = max_element(itfirst,(itfirst+100 > itend?itend:itfirst+100));
            itlast = std::find_if(peak_it,itend,std::bind(std::less<Float_t>(),std::placeholders::_1,fFallingThreshold));
            TCatPulseShape *out = NULL;
            if (fIsSparse) {
               out = (TCatPulseShape*) fOutput->ConstructedAt(hit->GetID());
               for (Int_t i=0; i!=fOutput->GetEntriesFast(); i++) {
                  // to avoid NULL dereference
                  if (fOutput->UncheckedAt(i)) continue;
                  (TCatPulseShape*) fOutput->ConstructedAt(i);
               }
               if (!out->TestQualityBit(TCatPulseShape::kInvalid)) break;
            } else {
               out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
            }
            std::vector<Float_t>& outSample = out->GetSample();
            std::vector<Float_t>& outClock = out->GetClock();
            
            //=========================================================================
#if 0
            std::vector<Float_t>::const_iterator ittmp = sample.begin();
            const std::vector<Float_t> yabsy;
            Float_t y    = 0.;
            Float_t yabs = 0.;
            for (; ittmp != itbegin+11; ittmp++) {
               y = sample.at(std::distance(itbegin,ittmp));
               yabs = TMath::Abs(y);
               //printf("Evt = %d, Hit = %d, y = %4.1f, yabs = %4.1f\n",fEvt,iHit,y,yabs);
               yabsy.push_back(yabs-y);
            }
            Float_t YAbsY = std::accumulate(yabsy.begin(),yabsy.end(),0.);
#else
            std::vector<Float_t> yabs;
            std::vector<Float_t>::const_iterator ittmp = itfirst;
            for (; ittmp != itlast; ittmp++) {
               yabs.push_back(TMath::Abs(*ittmp));
               //printf("[transform Abs] y = %f, yabs = %f\n",*ittmp,TMath::Abs(*ittmp));
            }
            Float_t YAbs = std::accumulate(yabs.begin(),yabs.end(),0.);
            std::transform(yabs.begin(),yabs.end(),itfirst,yabs.begin(),std::minus<Float_t>());
            ittmp = yabs.begin();
            Float_t YAbsY = std::accumulate(yabs.begin(),yabs.end(),0.);
            //for (; ittmp != yabs.end(); ittmp++) {
            //   printf("[transform YAbs-Y] yabs - y = %f\n",*ittmp);
            //}
#endif
            if (fSaveHist) {
               fhYAbsYAbsY->Fill(YAbs,YAbsY);
               fhSigmaYAbsY->Fill(ped2,YAbsY);
               if (YAbsY>0.) fhSigmaYAbsY2->Fill(ped2,YAbsY);
            }
            //=========================================================================
            
            // prepare output
            hit->Copy(*out);
            Double_t a = 1.,b = 0.;
            char *fomula = NULL;
            // calculate timing by using extrapolation
            {
               std::vector<Float_t> clock;
               std::vector<Float_t>::const_iterator max_it = max_element(itfirst,itlast);
               int nclock = std::distance(itfirst,max_it);
               fhGraph2->Clear();
#if 1
               if (nclock>3) nclock -= 3;
               fhGraph2->Set(nclock);
               for (int i = 0; i != nclock; i++) {
                  clock.push_back(i);
                  fhGraph2->SetPoint(i,std::distance(itbegin,itfirst+i),*(itfirst+i));
               }
               Double_t xy = std::inner_product(itfirst,itfirst+nclock,clock.begin(),0.);
               Double_t xx = std::inner_product(clock.begin(),clock.end(),clock.begin(),0.);
               Double_t x  = std::accumulate(clock.begin(),clock.end(),0.);
               Double_t y  = std::accumulate(itfirst,itfirst+nclock,0.);
               a = (nclock * xy - x * y) / (nclock * xx - x*x);
               b = (xx*y - xy * x) / (nclock * xx - x*x);

               rttiming = std::distance(itbegin,itfirst);
               offset  = hit->GetOffset() + std::distance(itbegin,itfirst);
               fomula = Form("%f*(x - %f) + %f",a,rttiming,b);
#else
               std::vector<Float_t>::const_iterator it1 = itfirst + (nclock/2 - 1);
               std::vector<Float_t>::const_iterator it2 = itfirst + nclock/2;
               std::vector<Float_t>::const_iterator it3 = itfirst + (nclock/2 + 1);
               std::vector<Float_t>::const_iterator ittmp = it1;
               printf("Evt = %d, Hit = %d, nclock = %d, itfirst = %zd, it1 = %zd, it2 = %zd, it3 = %zd\n",fEvt,iHit,nclock,std::distance(itbegin,itfirst),std::distance(itbegin,it1),std::distance(itbegin,it2),std::distance(itbegin,it3));
               for (; ittmp != it3; ittmp++) {
                  clock.push_back(std::distance(itbegin,ittmp));
               }
               printf("sample1 : (%zd,%f), sample2 : (%zd,%f), sample3 : (%zd,%f)\n",std::distance(itbegin,it1),*it1,std::distance(itbegin,it2),*it2,std::distance(itbegin,it3),*it3);
               Double_t xy = std::inner_product(it1,it3,clock.begin(),0.);
               Double_t xx = std::inner_product(clock.begin(),clock.end(),clock.begin(),0.);
               Double_t x  = std::accumulate(clock.begin(),clock.end(),0.);
               Double_t y  = std::accumulate(it1,it3,0.);
               nclock = 3;
               a = (nclock * xy - x * y) / (nclock * xx - x*x);
               Double_t b2 = 0.;
               b2 = (xx*y - xy * x) / (nclock * xx - x*x);
               b = (*it3) - a * std::distance(itbegin,it3);
               printf("a = %f, b = %f, b2 = %f\n",a,b,b2);
               fhGraph2->Set(nclock);
               fhGraph2->SetPoint(0,std::distance(itbegin,it1),*it1);
               fhGraph2->SetPoint(1,std::distance(itbegin,it2),*it2);
               fhGraph2->SetPoint(2,std::distance(itbegin,it3),*it3);
               rttiming = std::distance(itbegin,itfirst);
               //rttiming = std::distance(itbegin,itfirst) + b/a;
               offset   = hit->GetOffset() + std::distance(itbegin,itfirst) + b/a;
               fomula = Form("%f*x + %f",a,b);
#endif
            }
            //=========================================================================

            /* offset gate */
            if ((rttiming<fCutLowOffset) || (fCutHighOffset<rttiming)) {
               if (itlast!=itend) {
                  it = itlast + 1;
                  continue;
               } else {
                  break;
               }
            }
            //=========================================================================

            out->SetOffset(hit->GetOffset()+std::distance(itbegin,itfirst));

            // risetime
            std::vector<Float_t>::const_iterator itmax;
            itmax  = std::max_element(itfirst,itlast);
            out->SetRiseTime(std::distance(itfirst,itmax));

            itfirst = (itfirst - fPrePulse) > itbegin ? itfirst - fPrePulse: itbegin;
            itlast  = (itlast  + fPostPulse) < itend   ? itlast + fPostPulse : itend;
            it = itlast;

            out->SetCharge(std::accumulate(itfirst,itlast,0.));
            out->ResetQualityBit(TCatPulseShape::kInvalid);

            //=========================================================================

            /* charge gate */
            q = std::accumulate(itfirst,itlast,0.);
            if (fCutNegativeCharge) if (q<0) continue;
            if ((q<fCutLowCharge) || (fCutHighCharge<q)) {
               out->SetQualityBit(TCatPulseShape::kInvalid);
               if (fDebug) fprintf(fp,"Invalid: Evt = %d, iHit = %d, id = %d, charge = %0.2f\n",fEvt,iHit,hit->GetID(),q);
               continue;
            }
            //=========================================================================

            if (itlast - itfirst > fMaxLength) {
               out->SetQualityBit(TCatPulseShape::kLongPulse);
               out->SetQualityBit(TCatPulseShape::kInvalid);
               if (fDebug) fprintf(fp,"Invalid: Evt = %d, iHit = %d, id = %d, num = %zd, Long pulse\n",fEvt,iHit,hit->GetID(),std::distance(itfirst,itlast));
            }
            if (out->GetOffset() < fOffsetRange[0] || out->GetOffset() > fOffsetRange[1]) {
               out->SetQualityBit(TCatPulseShape::kOutOfOffset);
               out->SetQualityBit(TCatPulseShape::kInvalid);
               if (fDebug) fprintf(fp,"Invalid: Evt = %d, iHit = %d, id = %d, off = %4.1f, offset is out of range\n",fEvt,iHit,hit->GetID(),out->GetOffset());
            }

            // optional output
            if (fSavePulse) {
               int num = itlast - itfirst;
               outSample.resize(num);
               outClock.resize(num);
               std::copy(itfirst,itlast,outSample.begin());
               for (int i = 0; i!= num; i++) {
                  outClock[i] = (double)i;
               }
               out->SetNumSample(num);

               if (fDrawFig) {
                  TObject *hitobj = hit;
                  fPulseDrawer->SetPulse(*hitobj);
                  fPulseDrawer->SetEvent(fEvt,iHit);
                  fPulseDrawer->SetViewID(fSaveID);
                  fPulseDrawer->SetGraphRange(fGraphXmin,fGraphXmax,fGraphYmin,fGraphYmax);
                  fPulseDrawer->SetLine(rttiming,0.,rttiming,10.);
                  fPulseDrawer->SetLine2(fGraphXmin,fRisingThreshold,fGraphXmax,fRisingThreshold);
                  //fPulseDrawer->SetLine2((-b)/a+rttiming,fGraphYmin,(-b)/a+rttiming,fGraphYmax);
                  //fPulseDrawer->SetLine2((fGraphYmin-b)/a+std::distance(itbegin,itfirst),fGraphYmin,(fGraphYmax-b)/a+std::distance(itbegin,itfirst),fGraphYmax);
                  //fPulseDrawer->SetTF1(0,fRisingThreshold);
                  //fPulseDrawer->SetTF1(0,b);
                  //fPulseDrawer->SetTF1(1,a);
                  //printf("fomula = %f*x + %f\n",a,b);
                  fPulseDrawer->SetTF1(fomula);
                  fPulseDrawer->AddGraph(*fhGraph2);
                  fPulseDrawer->SetTextNDC(fomula,5,0.57,0.50);
                  fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f",fEvt,out->GetID(),out->GetOffset()),1);
                  fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
                  fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
                  fPulseDrawer->SetTextNDC(Form("RTh = %4.1f, FTh = %4.1f, PrePulse = %d",fRisingThreshold,fFallingThreshold,fPrePulse),5,0.30,0.035);
                  fPulseDrawer->SetTextNDC(Form("PostPulse = %d, MaxL = %d",fPostPulse,fMaxLength),5,0.30,0.01);
                  fPulseDrawer->SetTextNDC(Form("ped     = %f",ped),5,0.57,0.87);
                  fPulseDrawer->SetTextNDC(Form("sigma = %f",ped2),5,0.57,0.84);
                  fPulseDrawer->SetTextNDC(Form("itfirst  clock : %zd",std::distance(itbegin,itfirst)),5,0.57,0.81);
                  fPulseDrawer->SetTextNDC(Form("itmax  clock : %zd",std::distance(sample.begin(),itmax)),5,0.57,0.78);
                  fPulseDrawer->SetTextNDC(Form("itlast   clock : %zd",std::distance(itbegin,itlast)),5,0.57,0.75);
                  fPulseDrawer->SetTextNDC(Form("itend   clock : %zd",std::distance(itbegin,itend)),5,0.57,0.72);
                  fPulseDrawer->Draw();
               }

               if (fSaveHist) {
                  fhCharge->Fill(q);
                  fhOffset->Fill(rttiming);
                  fhOffsetCharge->Fill(rttiming,q);
                  fhSigmaCharge->Fill(ped2,q);
               }
            }
         }
      }
   }
   fEvt++;
}
