/* $Id:$ */
/**
 * @file   TCatCmdXval.cc
 * @date   Created : Feb 03, 2013
 * @author Kawase Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */

#include <iostream>

#include "TCatCmdXval.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <TArtCore.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TROOT.h>

#include <TCatHistManager.h>
#include <TCatPadManager.h>

#include <TArtCore.h>

using namespace std;

TCatCmdXval::TCatCmdXval()
  :fX(0),fY(0){
   SetName("xval");
   SetTitle("get coordinate by mouse");
}
TCatCmdXval::~TCatCmdXval()
{
}

TCatCmdXval* TCatCmdXval::Instance()
{
   static TCatCmdXval instance;
   return &instance;
}

Long_t TCatCmdXval::Cmd(vector<TString> tokens)
{
  Int_t n = tokens.size()-1;
  if ( n>3 ){
    printf("[xval] More than 3 values are ignored.\n");
    n = 3;
  }

  TString token("");

  for (Int_t i = 0; i!=n ; ++i){
    if(i) token += ", ";
    token += tokens[i+1];
  }

  TString s = TString::Format("TCatCmdXval::Instance()->Run(%s);",
			      token.Data());
  gROOT->ProcessLineFast(s);

  return 1;
}

Long_t TCatCmdXval::Run(TPad *pad, Double_t *x, Double_t *y) 
{
   if(pad==NULL) return 0;
   
   pad->AddExec("ex_xval","TCatCmdXval::Instance()->GetEvent()");
   pad->WaitPrimitive();
   TObject *obj = pad->GetListOfPrimitives()->Last();
   if (obj->IsA() != TLatex::Class()) {
      fX = fY = 0;
      return 0;
   }
   TLatex *latex = (TLatex*) obj;
   pad->GetListOfPrimitives()->RemoveLast();
   fX = latex->GetX();
   fY = latex->GetY();
   delete latex;

   Bool_t display = x==NULL && y==NULL;
   if(display){
     printf("[xval] X: %f, Y: %f\n",fX,fY);     
   }

   if(x!=NULL) *x = fX;
   if(y!=NULL) *y = fY;

   return 1;
}

Long_t TCatCmdXval::Run(Double_t *x, Double_t *y) 
{
   TPad *pad = (TPad*) gPad;
   if(pad==NULL) return 0;
   
   pad->AddExec("ex_xval","TCatCmdXval::Instance()->GetEvent()");
   pad->WaitPrimitive();
   TObject *obj = pad->GetListOfPrimitives()->Last();
   if (obj->IsA() != TLatex::Class()) {
      fX = fY = 0;
      return 0;
   }
   TLatex *latex = (TLatex*) obj;
   pad->GetListOfPrimitives()->RemoveLast();
   fX = latex->GetX();
   fY = latex->GetY();
   delete latex;

   Bool_t display = x==NULL && y==NULL;
   if(display){
     printf("[xval] X: %f, Y: %f\n",fX,fY);     
   }

   if(x!=NULL) *x = fX;
   if(y!=NULL) *y = fY;

   return 1;
}

void TCatCmdXval::GetEvent()
{
   int event = gPad->GetEvent();
   if (event != 11) return;
   gPad->DeleteExec("ex_xval");
   Int_t px = gPad->GetEventX();
   Double_t xx = gPad->AbsPixeltoX(px);
   Double_t x = gPad->PadtoX(xx);
   Int_t py = gPad->GetEventY();
   Double_t yy = gPad->AbsPixeltoY(py);
   Double_t y = gPad->PadtoY(yy);

   gPad->GetListOfPrimitives()->AddLast(new TLatex(x,y,""));
}
