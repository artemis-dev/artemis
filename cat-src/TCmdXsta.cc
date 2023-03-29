/* $Id:$ */
/**
 * @file   TCmdXsta.cc
 * @date   Created : 2015-03-29 16:07:00 JST 
 *   Last Modified : 2015-05-11 15:08:14 JST by tokieda

 * @author Tokieda Hiroshi <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */

#include <iostream>

#include "TCmdXsta.h"
#include <TCatCmdXval.h>
#include <TROOT.h>
#include <TClass.h>
#include <TGraph.h>
#include <TLatex.h>
#include <TLine.h>
#include <TH2.h>
#include <TCatHistManager.h>


using namespace std;

TCmdXsta::TCmdXsta()
   :fX1(0),fY1(0),fX2(0),fY2(0),fE(0)
{
   SetName("xsta");
   SetTitle("get entries in the range selected by mouse");
}
TCmdXsta::~TCmdXsta()
{
}

TCmdXsta* TCmdXsta::Instance()
{
   static TCmdXsta instance;
   return &instance;
}

Long_t TCmdXsta::Cmd(vector<TString> tokens)
{
  Int_t n = tokens.size()-1;
  if ( n>3 ){
    printf("[xsta] More than 3 staues are ignored.\n");
    n = 3;
  }

  TString token("");

  for (Int_t i = 0; i!=n ; ++i){
    if(i) token += ", ";
    token += tokens[i+1];
  }

  TString s = TString::Format("TCmdXsta::Instance()->Run(%s);",
			      token.Data());
  gROOT->ProcessLineFast(s);

  return 1;
 }

Long_t TCmdXsta::Run(TPad *pad, Double_t *x1, Double_t *y1, Double_t *x2, Double_t *y2) 
{
   if(pad==NULL) {
      printf("[xsta] Error: No pad\n");
      return 0;
   }
   Int_t crosshair = pad->GetCrosshair();
   pad->SetCrosshair(1);

   while (pad->GetPrimitive("TLine")) {
      TObject *line = pad->GetPrimitive("TLine");
      line->Delete();
   }
   pad->Modified();
   pad->Update();
   
   TCatCmdXval *xval = TCatCmdXval::Instance();
   xval->Run(pad,x1,y1);
   fX1 = xval->GetX();
   fY1 = xval->GetY();
   DrawLine(fX1,fY1);
   if(x1!=NULL) *x1 = fX1;
   if(y1!=NULL) *y1 = fY1;
   
   pad->Modified();
   pad->Update();
   
   xval->Run(pad,x2,y2);
   fX2 = xval->GetX();
   fY2 = xval->GetY();
   DrawLine(fX2,fY2);
   if(x2!=NULL) *x2 = fX2;
   if(y2!=NULL) *y2 = fY2;

   TObject *select = pad->GetPrimitive("htemp");
   if (pad->GetPrimitive("Graph")) {
      select = pad->GetPrimitive("Graph");
   }
   if (!select) select = TCatHistManager::Instance()->GetCurrent();
   if (!select) {
      pad->SetCrosshair(crosshair);
      return 0;
   } else {
      if (select->InheritsFrom("TH1") && !(select->InheritsFrom("TH2"))) {
         TH1 *h = (TH1*)select;
         Int_t binx1 = h->GetXaxis()->FindBin(fX1);
         Int_t binx2 = h->GetXaxis()->FindBin(fX2);
         if (binx2<=binx1) {
            fE = h->Integral(binx2,binx1);
         } else {
            fE = h->Integral(binx1,binx2);
         }
         printf("[xsta] Entries: %0.0f\n",fE);
      } else if (select->InheritsFrom("TH2")) {
         TH2 *h = (TH2*)select;
         Int_t binx1 = h->GetXaxis()->FindBin(fX1);
         Int_t biny1 = h->GetYaxis()->FindBin(fY1);
         Int_t binx2 = h->GetXaxis()->FindBin(fX2);
         Int_t biny2 = h->GetYaxis()->FindBin(fY2);
         if ((binx2<=binx1) && (biny1<biny2)) {
            fE = h->Integral(binx2,binx1,biny1,biny2);
         } else if ((binx2<=binx1) && (biny2<=biny1)) {
            fE = h->Integral(binx2,binx1,biny2,biny1);
         } else if ((binx1<binx2) && (biny2<=biny1)) {
            fE = h->Integral(binx1,binx2,biny2,biny1);
         } else {
            fE = h->Integral(binx1,binx2,biny1,biny2);
         }
         printf("[xysta] Entries: %0.0f\n",fE);
      } else if (select->InheritsFrom("TGraph")) {
         printf("[xysta] Under Construction (TTree;2D Draw;TGraph)\n");
         pad->SetCrosshair(crosshair);
         return 0;
      } else {
         select->IsA()->Print();
         printf("[xsta] Error: This Class of Object is NOT TH1 or TH2\n");
         pad->SetCrosshair(crosshair);
         return 0;
      }
   }
   pad->SetCrosshair(crosshair);
   return 1;
}

Long_t TCmdXsta::Run(Double_t *x1, Double_t *y1, Double_t *x2, Double_t *y2) 
{
   TPad *pad = (TPad*) gPad;
   if(pad==NULL) {
      printf("[xsta] Error: No pad\n");
      return 0;
   }
   Int_t crosshair = pad->GetCrosshair();
   pad->SetCrosshair(1);

   while (pad->GetPrimitive("TLine")) {
      TObject *line = pad->GetPrimitive("TLine");
      line->Delete();
   }
   pad->Modified();
   pad->Update();

   TCatCmdXval *xval = TCatCmdXval::Instance();
   xval->Run(pad,x1,y1);
   fX1 = xval->GetX();
   fY1 = xval->GetY();
   DrawLine(fX1,fY1);
   if(x1!=NULL) *x1 = fX1;
   if(y1!=NULL) *y1 = fY1;

   pad->Modified();
   pad->Update();
   
   xval->Run(pad,x2,y2);
   fX2 = xval->GetX();
   fY2 = xval->GetY();
   DrawLine(fX2,fY2);
   if(x2!=NULL) *x2 = fX2;
   if(y2!=NULL) *y2 = fY2;

   TObject *select = pad->GetPrimitive("htemp");
   if (pad->GetPrimitive("Graph")) {
      select = pad->GetPrimitive("Graph");
   }
   if (!select) select = TCatHistManager::Instance()->GetCurrent();
   if (!select) {
      pad->SetCrosshair(crosshair);
      return 0;
   } else {
      if (select->InheritsFrom("TH1") && !(select->InheritsFrom("TH2"))) {
         TH1 *h = (TH1*)select;
         Int_t binx1 = h->GetXaxis()->FindBin(fX1);
         Int_t binx2 = h->GetXaxis()->FindBin(fX2);
         if (binx2<=binx1) {
            fE = h->Integral(binx2,binx1);
         } else {
            fE = h->Integral(binx1,binx2);
         }
         printf("[xsta] Entries: %0.0f\n",fE);
      } else if (select->InheritsFrom("TH2")) {
         TH2 *h = (TH2*)select;
         Int_t binx1 = h->GetXaxis()->FindBin(fX1);
         Int_t biny1 = h->GetYaxis()->FindBin(fY1);
         Int_t binx2 = h->GetXaxis()->FindBin(fX2);
         Int_t biny2 = h->GetYaxis()->FindBin(fY2);
         if ((binx2<=binx1) && (biny1<biny2)) {
            fE = h->Integral(binx2,binx1,biny1,biny2);
         } else if ((binx2<=binx1) && (biny2<=biny1)) {
            fE = h->Integral(binx2,binx1,biny2,biny1);
         } else if ((binx1<binx2) && (biny2<=biny1)) {
            fE = h->Integral(binx1,binx2,biny2,biny1);
         } else {
            fE = h->Integral(binx1,binx2,biny1,biny2);
         }
         printf("[xysta] Entries: %0.0f\n",fE);
      } else if (select->InheritsFrom("TGraph")) {
         printf("[xysta] Under Construction (TTree;2D Draw;TGraph)\n");
         pad->SetCrosshair(crosshair);
         return 0;
      } else {
         select->IsA()->Print();
         printf("[xsta] Error: This Class of Objct is NOT TH1 or TH2\n");
         pad->SetCrosshair(crosshair);
         return 0;
      }
   }
   pad->SetCrosshair(crosshair);
   return 1;
}

void TCmdXsta::DrawLine(Double_t x, Double_t y)
{
   TObject *select = gPad->GetPrimitive("htemp");
   if (!select) select = TCatHistManager::Instance()->GetCurrent();
   if (!select) {
      return;
   } else {
      if (select->InheritsFrom("TH1")) {
         Float_t uymin = gPad->GetUymin();
         Float_t uymax = gPad->GetUymax();
         TLine *l = new TLine(x,uymin,x,uymax);
         l->SetLineColor(kRed);
         l->SetLineStyle(2);
         l->SetLineWidth(3);
         l->Draw();
         if (select->InheritsFrom("TH2")) {
            Float_t uxmin = gPad->GetUxmin();
            Float_t uxmax = gPad->GetUxmax();
            TLine *l2 = new TLine(uxmin,y,uxmax,y);
            l2->SetLineColor(kRed);
            l2->SetLineStyle(2);
            l2->SetLineWidth(3);
            l2->Draw();
         }
      } else {
         printf("[xsta] Error: Not inherit from TH1\n");
         return;
      }
   }
}

void TCmdXsta::Help()
{
   printf("DESCRIPTION:\n");
   printf("   This command can get the entries in TH1x or TH2x histograms.\n");
   printf("   But the following histograms are only supported.\n");
   printf("     - tree->Draw()\n");
   printf("        Only supported histname is \"htemp\", which is default name of histograms created by tree.\n");
   printf("        And histograms drawn by TH1x class are only supported. TH2x class is NOT supported.\n");
   printf("     - hist->Draw()\n");
   printf("        Histograms created from TCatHistManager's objects. Both TH1x and TH2x classes are supported.\n");
   printf("\n");
}
