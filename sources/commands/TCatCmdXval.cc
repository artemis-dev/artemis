/* $Id:$ */
/**
 * @file   TCatCmdXval.cc
 * @date   Created : Feb 03, 2013
 * @author Kawase Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */


#include "TCatCmdXval.h"

#include <TPad.h>
#include <TROOT.h>
#include <TMath.h>
#include <TObjArray.h>

using namespace std;

TCatCmdXval::TCatCmdXval()
  :fX(0),fY(0) {
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
  TString token("");
  const Int_t MAX_TOKEN = 3;

  for (Int_t i = 1, nToken = TMath::Min((Int_t)tokens.size(),MAX_TOKEN);
       i != nToken ; ++i){
     if (i != 1) token += ",";
     token += tokens[i];
  }

  const TString s = TString::Format("TCatCmdXval::Instance()->Run(%s);",
				    token.Data());
  gROOT->ProcessLineFast(s);

  return 1;
}

Long_t TCatCmdXval::Run(TPad *pad, Double_t *x, Double_t *y)
{
   if(!pad) return 0;
   pad->cd();

   pad->AddExec("ex_xval","TCatCmdXval::Instance()->GetEvent()");
   if (TObject *const obj = gPad->WaitPrimitive("xval")) {
      gPad->GetListOfPrimitives()->Remove(obj);
      delete obj;
   } else {
      fX = fY = 0;
      return 0;
   }

   if(!x && !y){
      // display coordinates when both of x and y are NULL.
      printf("[xval] X: %f, Y: %f\n",fX,fY);
   } else {
      if(x) *x = fX;
      if(y) *y = fY;
   }

   return 1;
}

Long_t TCatCmdXval::Run(Double_t *x, Double_t *y)
{
   return Run((TPad*)gPad,x,y);
}

void TCatCmdXval::GetEvent()
{
   const int event = gPad->GetEvent();
   if (event != kButton1Up) return;
   gPad->DeleteExec("ex_xval");
   const Int_t px = gPad->GetEventX();
   const Double_t xx = gPad->AbsPixeltoX(px);
   const Double_t x = gPad->PadtoX(xx);
   const Int_t py = gPad->GetEventY();
   const Double_t yy = gPad->AbsPixeltoY(py);
   const Double_t y = gPad->PadtoY(yy);

   fX = x;
   fY = y;
   gPad->GetListOfPrimitives()->AddLast(new TNamed("xval","xval"));
}
