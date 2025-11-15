/* $Id:$ */
/**
 * @file   TCatAtSaveChargePatternProcessor.cc
 * @date   Created : 2015-05-09 12:48:12 JST
 *   Last Modified : 2017-12-23 21:21:24 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#include "TCatAtSaveChargePatternProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TSystem.h>
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

ClassImp(art::TCatAtSaveChargePatternProcessor)


art::TCatAtSaveChargePatternProcessor::TCatAtSaveChargePatternProcessor()
: fPadX(NULL),fPadY(NULL),fReadoutPadName(""),fPadArray(NULL)
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName,defInput);
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
   RegisterInputInfo("ReadoutPad","Name of readout pad",fReadoutPadName,TString("readoutPad"),&fPadArray,art::TCatReadoutPadArray::Class_Name());
   RegisterProcessorParameter("CanvasX","X Position of Canvas",fCanvasX,(Int_t)0);
   RegisterProcessorParameter("CanvasY","Y Position of Canvas",fCanvasY,(Int_t)0);
   RegisterProcessorParameter("CanvasW","Width of Canvas",fCanvasW,(Int_t)650);
   RegisterProcessorParameter("CanvasH","Hight of Canvas",fCanvasH,(Int_t)670);
}
art::TCatAtSaveChargePatternProcessor::~TCatAtSaveChargePatternProcessor()
{
   if (fCanvas)   delete fCanvas;
}

void art::TCatAtSaveChargePatternProcessor::Init(art::TEventCollection *col) {
   if (!fPadArray) {
      SetStateError("Pad configuration is not initialized");
      return;
   }
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
   gStyle->SetOptLogz(0);
   gStyle->SetTextSize(0.5);
   fCanvas = new TCanvas(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
   fHistPad = (*fPadArray)->CreateHist(fInputColName.front().Data(),"Charge Pattern",-50.,50.,-50.,50.);
   fHistPad->Draw();
   fEvt = 0;
   t = new TText(0.1,0.1,"");
   fDatime   = new TDatime();
   kDiffTime = ((Long_t)gSystem->Now())/1000 - fDatime->Convert();
   fPadX = new Double_t[400];
   fPadY = new Double_t[400];
}

void art::TCatAtSaveChargePatternProcessor::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   TClonesArray *input = *fInput;
   Int_t nHits = input->GetEntriesFast();
   fHistPad->ClearBinContents();
   if (fFillClsFlag) { // cls fill mode
      std::vector<TCatPulseShape*> icls;
      std::vector<std::vector<TCatPulseShape*> > cls;
      Int_t ncls = 0;
      Int_t cls_i_tmp = 0;
      Int_t cls_id = 0;
      if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
      if (fDetailDebug) if (nHits) printf("Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
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
         if (fDetailDebug) printf("iHit = %3d, id = %3d, cls_id = %2d, cls_i_tmp = %2d\n",iHit,id,cls_id,cls_i_tmp);
      }
      fCanvas->cd();
      for (Int_t icls=0; icls < ncls; icls++) {
         fHistPad->ClearBinContents();
         std::vector<TCatPulseShape*>::iterator it = cls[icls].begin();
         std::vector<TCatPulseShape*>::iterator itend = cls[icls].end();
         for (; it != itend; it++) {
            Int_t id = (*it)->GetID();
            cls_id = (*it)->GetAuxID();
            Double_t q = (*it)->GetCharge();
            fPadX[id] = (*it)->GetX();
            fPadY[id] = (*it)->GetZ();
            fHistPad->Fill(fPadX[id],fPadY[id],q);
         }
         if (fDebug) fprintf(fp,"Evt = %4d, ncls = %3d, icls = %3d, cls_id = %3d, nicls = %3zd\n",fEvt,ncls,icls,cls_id,cls[icls].size());
         if (fDetailDebug) printf("Evt = %4d, ncls = %3d, icls = %3d, cls_id = %3d, nicls = %3zd\n",fEvt,ncls,icls,cls_id,cls[icls].size());
         if (fHistPad->GetEntries()) {
            Long64_t now_msec = gSystem->Now();
            fDatime->Set(now_msec/1000-kDiffTime);
            if (fSaveFigFlag) {
               std::stringstream out_name;
               {
                  std::string FigNamePath(fFigFile);
                  if (fSaveEvt != -1) {
                     if (fEvt == fSaveEvt) {
                        out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_id << "_chargepattern.png";
                        if (!fNoColorFig) {
                           fHistPad->Draw("colz");
                        } else {
                           fHistPad->Draw();
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
                     out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_id << "_chargepattern.png";
                     if (!fNoColorFig) {
                        fHistPad->Draw("colz");
                     } else {
                        fHistPad->Draw();
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
                        fHistPad->Draw("colz");
                     } else {
                        fHistPad->Draw();
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
                     fHistPad->Draw("colz");
                  } else {
                     fHistPad->Draw();
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
      if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
      if (fDetailDebug) if (nHits) printf("Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *d = (TCatPulseShape*) (input)->At(iHit);
         Int_t id = d->GetID();
         Double_t q = d->GetCharge();
         fPadX[id] = d->GetX();
         fPadY[id] = d->GetZ();
         fHistPad->Fill(fPadX[id],fPadY[id],q);
      }
      fCanvas->cd();
      if (fHistPad->GetEntries()) {
         Long64_t now_msec = gSystem->Now();
         fDatime->Set(now_msec/1000-kDiffTime);
         if (fSaveFigFlag) {
            std::stringstream out_name;
            {
               std::string FigNamePath(fFigFile);
               if (fSaveEvt != -1) {
                  if (fEvt == fSaveEvt) {
                     out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_chargepattern.png";
                     if (!fNoColorFig) {
                        fHistPad->Draw("colz");
                     } else {
                        fHistPad->Draw();
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
                  out_name << FigNamePath << fInputColName.front().Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_chargepattern.png";
                  if (!fNoColorFig) {
                     fHistPad->Draw("colz");
                  } else {
                     fHistPad->Draw();
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
                     fHistPad->Draw("colz");
                  } else {
                     fHistPad->Draw();
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
                  fHistPad->Draw("colz");
               } else {
                  fHistPad->Draw();
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
   fEvt++;
}
