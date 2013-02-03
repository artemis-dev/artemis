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

#include <TCatHistManager.h>
#include <TCatPadManager.h>

#include <TArtCore.h>

using namespace std;

TCatCmdXval::TCatCmdXval()
{
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
   Double_t x,y;
   Long_t ret = Run(x,y);
   printf("[xval] X: %f, Y: %f\n",x,y);
   return ret;
}

Long_t TCatCmdXval::Run(Double_t& x, Double_t& y) 
{
   if(gPad==NULL) return 1;

   gPad->AddExec("ex_xval","TCatCmdXval::Instance()->GetEvent()");
   gPad->WaitPrimitive();
   TObject *obj = gPad->GetListOfPrimitives()->Last();
   if (obj->IsA() != TLatex::Class()) {
      x = y = 0;
      return 0;
   }
   TLatex *latex = (TLatex*) obj;
   gPad->GetListOfPrimitives()->RemoveLast();
   x = latex->GetX();
   y = latex->GetY();
   delete latex;

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
