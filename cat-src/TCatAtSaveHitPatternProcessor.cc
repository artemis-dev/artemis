/* $Id:$ */
/**
 * @file   TCatAtSaveHitPatternProcessor.cc
 * @date   Created : 2013
 *   Last Modified : 2016-08-31 12:15:44 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtSaveHitPatternProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TEventHeader.h>
#include <TSystem.h>
#include <TH2Poly.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <unistd.h>

ClassImp(art::TCatAtSaveHitPatternProcessor)


art::TCatAtSaveHitPatternProcessor::TCatAtSaveHitPatternProcessor()
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName,defInput);
   //RegisterProcessorParameter("HistName","HistName",fHistName,TString("hHitPattern"));
   RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
   RegisterProcessorParameter("SaveFigFlag","Flag of Save Figure",fSaveFigFlag,false);
   RegisterProcessorParameter("FillClsFlag","Flag of Fill Each Cluster",fFillClsFlag,false);
   RegisterProcessorParameter("NoColorFigFlag","Flag of Fill No Color Figure",fNoColorFig,false);
   RegisterProcessorParameter("FigFilePath","Path of Figure File",fFigFilePath,TString("./"));
   RegisterProcessorParameter("AddFigFilePath","Add Path of Figure File",fAddFigFilePath,TString("./"));
   RegisterProcessorParameter("SaveEvtNum","Number of Save Event",fSaveEvt,-1);
   RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
   RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
   RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
   RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("savehitpatternproc.log"));
   RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
   RegisterInputCollection("EventHeader","name of event header (default : eventheader)",fNameEventHeader,TString("eventheader"),
                             &fEventHeader,art::TEventHeader::Class_Name());
   RegisterProcessorParameter("CanvasX","X Position of Canvas",fCanvasX,(Int_t)0);
   RegisterProcessorParameter("CanvasY","Y Position of Canvas",fCanvasY,(Int_t)0);
   RegisterProcessorParameter("CanvasW","Width of Canvas",fCanvasW,(Int_t)650);
   RegisterProcessorParameter("CanvasH","Hight of Canvas",fCanvasH,(Int_t)670);
}
art::TCatAtSaveHitPatternProcessor::~TCatAtSaveHitPatternProcessor()
{
   delete fCanvas;
   delete fHPadEvt;
}

void art::TCatAtSaveHitPatternProcessor::Init(art::TEventCollection *col) {
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      exit(1);
   }
   // Prepare Log/Fig File
   if (fDebug || fDetailDebug) {
      const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
      fp = fopen(LogNamePath,"w");
      if (fp == NULL) {
         Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
         exit(1);
      }
   }
   if (fSaveFigFlag) fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fInputColName.front().Data(),fAddFigFilePath.Data());
   
   fInput = (TClonesArray**) obj->GetObjectRef();
   gDirectory = gROOT;
   gStyle->SetOptStat(11);
   gStyle->SetStatX(0.900);
   gStyle->SetStatY(0.980);
   gStyle->SetStatH(0.159);
   gStyle->SetStatW(0.150);
   gStyle->SetTitleYOffset(1.5);
   fHPadEvt = DrawPad(fNPad,fPadX,fPadY);
   fHPadEvt->SetName(fInputColName.front().Data());
   fCanvas = new TCanvas(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
   t = new TText(0.1,0.1,"");
   fDatime   = new TDatime();
   kDiffTime = ((Long_t)gSystem->Now())/1000 - fDatime->Convert();
}

void art::TCatAtSaveHitPatternProcessor::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   fEvt = (*fEventHeader)->GetEventNumber();
   TClonesArray *input = *fInput;
   Int_t nHits = input->GetEntriesFast();
   if (fDebug) fprintf(fp,"Evt = %d, nHits = %d\n",fEvt,nHits);
   fHPadEvt->ClearBinContents();
   if (fFillClsFlag) { // cls fill mode
      std::vector<TCatPulseShape*> icls;
      std::vector<std::vector<TCatPulseShape*> > cls;
      Int_t ncls = 0;
      Int_t cls_i_tmp = 0;
      Int_t cls_id = 0;
      if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) (input)->At(iHit);
         Int_t id = hit->GetID();
         cls_id = hit->GetAuxID();

         if (iHit==0) ncls = 1;
         if ((iHit==0) || (cls_i_tmp==cls_id)) {
            icls.push_back(hit);
         } else {
            cls.push_back(icls);
            icls.clear();
            icls.push_back(hit);
            ncls++;
         }
         cls_i_tmp = cls_id;
         if (iHit == nHits-1) { // last hit
            cls.push_back(icls);
         }
         if (fDebug) fprintf(fp,"iHit = %3d, id = %3d, cls_id = %2d, cls_i_tmp = %2d\n",iHit,id,cls_id,cls_i_tmp);
      }
      fCanvas->cd();
      for (Int_t icls=0; icls < ncls; icls++) {
         fHPadEvt->ClearBinContents();
         std::vector<TCatPulseShape*>::iterator it = cls[icls].begin();
         std::vector<TCatPulseShape*>::iterator itend = cls[icls].end();
         for (; it != itend; it++) {
            Int_t id = (*it)->GetID();
            cls_id = (*it)->GetAuxID();
            fHPadEvt->Fill(fPadX[id],fPadY[id]);
         }
         if (fDebug) fprintf(fp,"Evt = %4d, ncls = %3d, icls = %3d, cls_id = %3d, nicls = %3zd\n",fEvt,ncls,icls,cls_id,cls[icls].size());
         if (fDetailDebug) printf("Evt = %4d, ncls = %3d, icls = %3d, cls_id = %3d, nicls = %3zd\n",fEvt,ncls,icls,cls_id,cls[icls].size());
         fHPadEvt->SetMaximum(10);
         fHPadEvt->SetMinimum(0);
         if (fHPadEvt->GetEntries()) {
            Long64_t now_msec = gSystem->Now();
            fDatime->Set(now_msec/1000-kDiffTime);
            if (fSaveFigFlag) {
               std::stringstream out_name;
               {
                  std::string FigNamePath(fFigFile);
                  if (fSaveEvt != -1) {
                     if (fEvt == fSaveEvt) {
                        out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_id << ".png";
                        if (!fNoColorFig) {
                           fHPadEvt->Draw("colz");
                        } else {
                           fHPadEvt->Draw();
                        }
                        t->SetTextColor(kBlue);
                        t->SetTextSize(0.03);
                        if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                        t->DrawTextNDC(0.10,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_id));
                        if (t) t->SetTextColor(kBlack);
                        if (t) t->SetTextSize(0.02);
                        if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                        gPad->SaveAs(out_name.str().c_str());
                        fCanvas->Update();
                        if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                     }
                  } else {
                     out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_id << ".png";
                     if (!fNoColorFig) {
                        fHPadEvt->Draw("colz");
                     } else {
                        fHPadEvt->Draw();
                     }
                     t->SetTextColor(kBlue);
                     t->SetTextSize(0.03);
                     if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                     t->DrawTextNDC(0.10,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_id));
                     if (t) t->SetTextColor(kBlack);
                     if (t) t->SetTextSize(0.02);
                     if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                     gPad->SaveAs(out_name.str().c_str());
                     fCanvas->Update();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               }
            } else {
               if (fSaveEvt != -1) {
                  if (fEvt == fSaveEvt) {
                     if (!fNoColorFig) {
                        fHPadEvt->Draw("colz");
                     } else {
                        fHPadEvt->Draw();
                     }
                     t->SetTextColor(kBlue);
                     t->SetTextSize(0.03);
                     if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                     t->DrawTextNDC(0.10,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_id));
                     if (t) t->SetTextColor(kBlack);
                     if (t) t->SetTextSize(0.02);
                     if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                     fCanvas->Update();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               } else {
                  if (!fNoColorFig) {
                     fHPadEvt->Draw("colz");
                  } else {
                     fHPadEvt->Draw();
                  }
                  t->SetTextColor(kBlue);
                  t->SetTextSize(0.03);
                  if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                  t->DrawTextNDC(0.10,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_id));
                  if (t) t->SetTextColor(kBlack);
                  if (t) t->SetTextSize(0.02);
                  if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                  fCanvas->Update();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            }
         }
      }
   } else { // no cls fill mode
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *d = (TCatPulseShape*) (input)->At(iHit);
         if (!d) continue;
         Int_t id = d->GetID();
         if (fDebug) fprintf(fp,"iHit = %d, id = %d, num = %d, offset = %4.1f\n",iHit,id,d->GetNumSample(),d->GetOffset());
         fHPadEvt->Fill(fPadX[id],fPadY[id]);
      }
      fCanvas->cd();
      fHPadEvt->SetMaximum(10);
      fHPadEvt->SetMinimum(0);
      if (fHPadEvt->GetEntries()) {
         Long64_t now_msec = gSystem->Now();
         fDatime->Set(now_msec/1000-kDiffTime);
         if (fSaveFigFlag) {
            std::stringstream out_name;
            {
               std::string FigNamePath(fFigFile);
               if (fSaveEvt != -1) {
                  if (fEvt == fSaveEvt) {
                     out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << ".png";
                     if (!fNoColorFig) {
                        fHPadEvt->Draw("colz");
                     } else {
                        fHPadEvt->Draw();
                     }
                     t->SetTextColor(kBlue);
                     t->SetTextSize(0.03);
                     if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                     t->DrawTextNDC(0.10,0.91,Form("Evt = %5d",fEvt));
                     if (t) t->SetTextColor(kBlack);
                     if (t) t->SetTextSize(0.02);
                     if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                     gPad->SaveAs(out_name.str().c_str());
                     fCanvas->Update();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               } else {
                  out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << ".png";
                  if (!fNoColorFig) {
                     fHPadEvt->Draw("colz");
                  } else {
                     fHPadEvt->Draw();
                  }
                  t->SetTextColor(kBlue);
                  t->SetTextSize(0.03);
                  if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                  t->DrawTextNDC(0.10,0.91,Form("Evt = %5d",fEvt));
                  if (t) t->SetTextColor(kBlack);
                  if (t) t->SetTextSize(0.02);
                  if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                  gPad->SaveAs(out_name.str().c_str());
                  fCanvas->Update();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            }
         } else {
            if (fSaveEvt != -1) {
               if (fEvt == fSaveEvt) {
                  if (!fNoColorFig) {
                     fHPadEvt->Draw("colz");
                  } else {
                     fHPadEvt->Draw();
                  }
                  t->SetTextColor(kBlue);
                  t->SetTextSize(0.03);
                  if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                  t->DrawTextNDC(0.10,0.91,Form("Evt = %5d",fEvt));
                  if (t) t->SetTextColor(kBlack);
                  if (t) t->SetTextSize(0.02);
                  if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                  fCanvas->Update();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            } else {
               if (!fNoColorFig) {
                  fHPadEvt->Draw("colz");
               } else {
                  fHPadEvt->Draw();
               }
               t->SetTextColor(kBlue);
               t->SetTextSize(0.03);
               if (fRunInfo) t->DrawTextNDC(0.10,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
               t->DrawTextNDC(0.10,0.91,Form("Evt = %5d",fEvt));
               if (t) t->SetTextColor(kBlack);
               if (t) t->SetTextSize(0.02);
               if (t) t->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
               fCanvas->Update();
               if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
            }
         }
      }
   }
   //fEvt++;
}


TH2Poly* art::TCatAtSaveHitPatternProcessor::DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY) 
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
