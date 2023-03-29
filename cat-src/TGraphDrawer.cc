/* $Id:$ */
/**
 * @file   TGraphDrawer.cc
 * @date   Created : 2015-04-05 17:07:04 JST
 *   Last Modified : 2016-02-16 22:35:35 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#include "TGraphDrawer.h"
#include <TCatPulseShape.h>
#include <TSystem.h>
#include <TAxis.h>
#include <TH1F.h>
#include <sstream>
#include <iomanip>
#include <unistd.h>

using art::TGraphDrawer;


TGraphDrawer::TGraphDrawer()
   : fCanvas(NULL),fCanvasName(NULL),fCanvasTitle(NULL),fCanvasWtopx(0),fCanvasWtopy(0),
     fCanvasWw(0),fCanvasWh(0),fhGraph(NULL),fhGraph2(NULL),fhTF1(NULL),fNText(0),fss(NULL),fTextX(NULL),
     fTextY(NULL),ft(NULL),fWaitTime(0),fEvt(0),fHit(0),fViewEvt(-1),fViewHit(-1),
     fSaveFig(kFALSE),fFigFilePath("../figs/"),fRunHeaders(NULL),fRunInfo(NULL),fRunName(""),
     fRunNumber(0),kDiffTime(0),
     fLine(NULL),fLineX1(0),fLineX2(0),fLineY1(0),fLineY2(0),
     fLine2(NULL),fLine2X1(0),fLine2X2(0),fLine2Y1(0),fLine2Y2(0),
     fGraph_xmin(0.),fGraph_xmax(100.),fGraph_ymin(-2.),fGraph_ymax(15.),fChGraphRange(kFALSE)
{
   fCanvas   = new TCanvas("graphdrawer","Graph Drawer",0,0,1000,1050);
   fhGraph   = new TGraph;
   fhGraph2  = new TGraph;
   ft        = new TText();
   fss       = new const char*[kMaxNText];
   fTextX    = new Double_t[kMaxNText];
   fTextY    = new Double_t[kMaxNText];
   fDatime   = new TDatime();
   kDiffTime = ((Long_t)gSystem->Now())/1000 - fDatime->Convert();
   //fLine     = new TLine();
}

TGraphDrawer::TGraphDrawer(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
   : fCanvas(NULL),fCanvasName(name),fCanvasTitle(title),fCanvasWtopx(wtopx),fCanvasWtopy(wtopy),
     fCanvasWw(ww),fCanvasWh(wh),fhGraph(NULL),fhGraph2(NULL),fhTF1(NULL),fNText(0),fss(NULL),fTextX(NULL),
     fTextY(NULL),ft(NULL),fWaitTime(0),fEvt(0),fHit(0),fViewEvt(-1),fViewHit(-1),
     fSaveFig(kFALSE),fFigFilePath("../figs/"),fRunHeaders(NULL),fRunInfo(NULL),fRunName(""),
     fRunNumber(0),kDiffTime(0),fLine(NULL),fLineX1(0),fLineX2(0),fLineY1(0),fLineY2(0),
     fLine2(NULL),fLine2X1(0),fLine2X2(0),fLine2Y1(0),fLine2Y2(0),
     fGraph_xmin(0.),fGraph_xmax(100.),fGraph_ymin(-2.),fGraph_ymax(15.),fChGraphRange(kFALSE)
{
   fCanvas   = new TCanvas(fCanvasName,fCanvasTitle,fCanvasWtopx,fCanvasWtopy,fCanvasWw,fCanvasWh);
   fhGraph   = new TGraph;
   fhGraph2  = new TGraph;
   ft        = new TText();
   fss       = new const char*[kMaxNText];
   fTextX    = new Double_t[kMaxNText];
   fTextY    = new Double_t[kMaxNText];
   fDatime   = new TDatime();
   kDiffTime = ((Long_t)gSystem->Now())/1000 - fDatime->Convert();
   //fLine     = new TLine();
}

TGraphDrawer::~TGraphDrawer()
{
   if (fCanvas) delete fCanvas;
   if (fhGraph) delete fhGraph;
   if (fhGraph2)delete fhGraph2;
   if (fhTF1)   delete fhTF1;
   if (ft)      delete ft;
   if (fss)     delete [] fss;
   if (fTextX)  delete [] fTextX;
   if (fTextY)  delete [] fTextY;
   if (fDatime) delete fDatime;
   if (fLine)   delete fLine;
   if (fLine2)  delete fLine2;
}

void TGraphDrawer::AddGraph(TGraph &obj)
{
   fhGraph2 = &obj;
}

void TGraphDrawer::Copy(TObject &obj) const
{
   TGraphDrawer& graphdw = (TGraphDrawer&) obj;
   graphdw.fCanvas       = new TCanvas();
   graphdw.fCanvasName   = fCanvasName;
   graphdw.fCanvasTitle  = fCanvasTitle;
   graphdw.fCanvasWtopx  = fCanvasWtopx;
   graphdw.fCanvasWtopy  = fCanvasWtopy;
   graphdw.fCanvasWw     = fCanvasWw;
   graphdw.fCanvasWh     = fCanvasWh;
   graphdw.fhGraph       = new TGraph();
   graphdw.fhGraph2      = new TGraph();
   graphdw.fhTF1         = new TF1();
   graphdw.ft            = new TText();
   graphdw.fEvt          = fEvt;
   graphdw.fWaitTime     = fWaitTime; 
   graphdw.fSaveFig      = fSaveFig;
   graphdw.fFigFilePath  = fFigFilePath;
   graphdw.fViewEvt      = fViewEvt;
   graphdw.fViewHit      = fViewHit;
   graphdw.fHit          = fHit;
   graphdw.fLine         = fLine;
   graphdw.fLineX1       = fLineX1;
   graphdw.fLineX2       = fLineX2;
   graphdw.fLineY1       = fLineY1;
   graphdw.fLineY2       = fLineY2;
   graphdw.fLine2        = fLine2;
   graphdw.fLine2X1      = fLine2X1;
   graphdw.fLine2X2      = fLine2X2;
   graphdw.fLine2Y1      = fLine2Y1;
   graphdw.fLine2Y2      = fLine2Y2;
}

void TGraphDrawer::Clear(Option_t *opt)
{
   fEvt         = 0;
   fWaitTime    = 0; 
   fSaveFig     = kFALSE;
   fFigFilePath = "../figs/";
   fViewEvt     = -1;
   fViewHit     = -1;
   fHit         = 0;
}

void TGraphDrawer::Draw(Option_t *opt)
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   
   fCanvas->cd();
   fhGraph->SetMarkerStyle(8);
   if (fhGraph2) fhGraph2->SetMarkerStyle(8);
   if (fhGraph2) fhGraph2->SetMarkerColor(kCyan);
   if (fChGraphRange) {
      TAxis *axis = fhGraph->GetXaxis();
      axis->SetLimits(fGraph_xmin,fGraph_xmax);
      fhGraph->GetHistogram()->SetMaximum(fGraph_ymax);
      fhGraph->GetHistogram()->SetMinimum(fGraph_ymin);
   }
   if (fhTF1) fhTF1->SetLineColor(kRed);
   if (fhTF1) fhTF1->SetLineStyle(2);
   if (ft) ft->SetTextColor(kBlue);
   if (ft) ft->SetTextSize(0.03);
   Long64_t now_msec = gSystem->Now();
   fDatime->Set(now_msec/1000-kDiffTime);
   if (fSaveFig) {
      std::stringstream out_name;
      {
         std::string FigNamePath(fFigFilePath);
         if (fViewEvt != -1) {
            if (fEvt == fViewEvt) {
               if (fViewHit != -1) {
                  if (fHit == fViewHit) {
                     if (fHit!=-1) out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_Hit" << fHit << ".png";
                     else out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << ".png";
                     fhGraph->Draw("opt AP");
                     if (fhGraph2) fhGraph2->Draw("P same");
                     if (fhTF1) fhTF1->Draw("opt same");
                     if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                     for (Int_t i=0; i<fNText; i++) {
                        if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
                     }
                     if (ft) ft->SetTextColor(kBlack);
                     if (ft) ft->SetTextSize(0.02);
                     if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                     if (fLine) {
                        fLineY1 = fGraph_ymax;
                        fLineY2 = fGraph_ymin;
                        fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
                     }
                     if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
                     gPad->SaveAs(out_name.str().c_str());
                     fCanvas->Update();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               } else {
                  if (fHit!=-1) out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_Hit" << fHit << ".png";
                  else out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << ".png";
                  fhGraph->Draw("opt AP");
                  if (fhGraph2) fhGraph2->Draw("P same");
                  if (fhTF1) fhTF1->Draw("opt same");
                  if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                  for (Int_t i=0; i<fNText; i++) {
                     if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
                  }
                  if (ft) ft->SetTextColor(kBlack);
                  if (ft) ft->SetTextSize(0.02);
                  if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                  if (fLine) {
                     fLineY1 = fGraph_ymax;
                     fLineY2 = fGraph_ymin;
                     fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
                  }
                  if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
                  gPad->SaveAs(out_name.str().c_str());
                  fCanvas->Update();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            }
         } else {
            if (fHit!=-1) out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_Hit" << fHit << ".png";
            else out_name << FigNamePath << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << ".png";
            fhGraph->Draw("opt AP");
            if (fhGraph2) fhGraph2->Draw("P same");
            if (fhTF1) fhTF1->Draw("opt same");
            if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
            for (Int_t i=0; i<fNText; i++) {
               if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
            }
            if (ft) ft->SetTextColor(kBlack);
            if (ft) ft->SetTextSize(0.02);
            if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
            if (fLine) {
               fLineY1 = fGraph_ymax;
               fLineY2 = fGraph_ymin;
               fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
               if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
            }
            gPad->SaveAs(out_name.str().c_str());
            fCanvas->Update();
            if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
         }
      }
   } else { // not save fig.
      if (fViewEvt != -1) {
         if (fEvt == fViewEvt) {
            if (fViewHit != -1) {
               if (fHit == fViewHit) {
                  fhGraph->Draw("opt AP");
                  if (fhGraph2) fhGraph2->Draw("P same");
                  if (fhTF1) fhTF1->Draw("opt same");
                  if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
                  for (Int_t i=0; i<fNText; i++) {
                     if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
                  }
                  if (ft) ft->SetTextColor(kBlack);
                  if (ft) ft->SetTextSize(0.02);
                  if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
                  if (fLine) {
                     fLineY1 = fGraph_ymax;
                     fLineY2 = fGraph_ymin;
                     fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
                  }
                  if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
                  fCanvas->Update();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            } else {
               fhGraph->Draw("opt AP");
               if (fhGraph2) fhGraph2->Draw("P same");
               if (fhTF1) fhTF1->Draw("opt same");
               if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
               for (Int_t i=0; i<fNText; i++) {
                  if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
               }
               if (ft) ft->SetTextColor(kBlack);
               if (ft) ft->SetTextSize(0.02);
               if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
               if (fLine) {
                  fLineY1 = fGraph_ymax;
                  fLineY2 = fGraph_ymin;
                  fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
               }
               if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
               fCanvas->Update();
               if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
            }
         }
      } else {
         fhGraph->Draw("opt AP");
         if (fhGraph2) fhGraph2->Draw("P same");
         if (fhTF1) fhTF1->Draw("opt same");
         if (ft && fRunInfo) ft->DrawTextNDC(0.70,0.94,Form("Run: %s%04d",fRunName.Data(),fRunNumber));
         for (Int_t i=0; i<fNText; i++) {
            if (ft) ft->DrawTextNDC(fTextX[i],fTextY[i],fss[i]);
         }
         if (ft) ft->SetTextColor(kBlack);
         if (ft) ft->SetTextSize(0.02);
         if (ft) ft->DrawTextNDC(0.75,0.01,Form("%d/%02d/%02d %02d:%02d:%02d JST",fDatime->GetYear(),fDatime->GetMonth(),fDatime->GetDay(),fDatime->GetHour(),fDatime->GetMinute(),fDatime->GetSecond()));
         if (fLine) {
            fLineY1 = fGraph_ymax;
            fLineY2 = fGraph_ymin;
            fLine->DrawLine(fLineX1,fLineY1,fLineX2,fLineY2);
         }
         if (fLine2) fLine2->DrawLine(fLine2X1,fLine2Y1,fLine2X2,fLine2Y2);
         fCanvas->Update();
         if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
      }
   }
   //if (fhTF1) fhTF1->ReleaseParameter(0);
   fNText = 0;
}

void TGraphDrawer::SetCanvas(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
{
   fCanvasName  = name;
   fCanvasTitle = title;
   fCanvasWtopx = wtopx;
   fCanvasWtopy = wtopy;
   fCanvasWw    = ww;
   fCanvasWh    = wh;
}

void TGraphDrawer::SetGraph(TGraph &obj)
{
   fhGraph = &obj;
}

void TGraphDrawer::SetLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
   fLine   = new TLine();
   fLine->SetLineColor(kGreen);
   fLineX1 = x1; 
   fLineY1 = y1; 
   fLineX2 = x2; 
   fLineY2 = y2;
}

void TGraphDrawer::SetLine2(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
   fLine2   = new TLine();
   fLine2->SetLineColor(kMagenta);
   fLine2X1 = x1; 
   fLine2Y1 = y1; 
   fLine2X2 = x2; 
   fLine2Y2 = y2;
}

void TGraphDrawer::SetOptions(Int_t waittime, Int_t viewevt, Bool_t save, TString filepath)
{
   fWaitTime    = waittime;
   fViewEvt     = viewevt;
   fSaveFig     = save;
   fFigFilePath = filepath;
}

void TGraphDrawer::SetTextNDC(const char* text, Int_t pos, Double_t ndc_x, Double_t ndc_y)
{
   fss[fNText] = text;

   switch(pos) {
   case 1: // Left-Up (default)
      fTextX[fNText] = 0.10;
      fTextY[fNText] = 0.94;
      break;
   case 2: // Left-Down
      fTextX[fNText] = 0.10;
      fTextY[fNText] = 0.03;
      break;
   case 3: // Right-Up
      fTextX[fNText] = 0.70;
      fTextY[fNText] = 0.94;
      break;
   case 4: // Right-Down
      fTextX[fNText] = 0.70;
      fTextY[fNText] = 0.03;
      break;
   case 5: // Anywhere
      fTextX[fNText] = ndc_x;
      fTextY[fNText] = ndc_y;
      break;
   }
   fNText++;
}

//void TGraphDrawer::SetTF1(Int_t param, Double_t value)
//{
//   if (!fhTF1) fhTF1 = new TF1("hTF1","[0]+[1]*x+[2]*x*x",0.,500.);
//   fhTF1->SetParameter(param,value);
//}
void TGraphDrawer::SetTF1(const char* fomula)
{
   fhTF1 = new TF1("hTF1",fomula,0.,500.);
   //fhTF1->SetParameter(param,value);
}
