/**
 * @file   TCmdSlope.cc
 * @brief  Obtain slope graphically
 *
 * @date   Created       : 2017-05-26 14:24:13 JST
 *         Last Modified : 2017-05-29 16:50:29 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdSlope.h"

#include <TCatCmdXval.h>
#include <TCmdComment.h>

#include <TPad.h>
#include <TF1.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TROOT.h>

using art::TCmdSlope;

ClassImp(TCmdSlope)

const char* TCmdSlope::kFuncNameBase = "f";
const char* TCmdSlope::kLabelNameBase = "l";

TCmdSlope::TCmdSlope()
{
   SetName("slo");
   SetTitle("get slope by mouse");
}

TCmdSlope::~TCmdSlope(){}

Long_t TCmdSlope::Cmd(vector<TString>)
{
   if (gPad == NULL) {
      return 1;
   }

   Double_t x1 = 0, y1 = 0, x2 = 0, y2 = 0;
   Info("Cmd","click the 1st point: ");
   TCatCmdXval::Instance()->Run(&x1,&y1);
   printf("X: %f, Y: %f\n",x1,y1);
   Info("Cmd","click the 2nd point: ");
   TCatCmdXval::Instance()->Run(&x2,&y2);
   printf("X: %f, Y: %f\n",x2,y2);

   const Double_t a  = (y2-y1)/(x2-x1);
   const Double_t y0 = y1 - x1*a;
   printf("a = %e, y0 = %e\n",a,y0);

   TCmdComment::Instance()->Run(
      TString::Format("[%s] a: %g, y0 = %g, 1/a = %g",
                      GetName(),a,y0,1./a));

   {
      TF1 *func;
      const TString func_name = TString::Format("%s_%s_%s",
                                                kFuncNameBase,
                                                GetName(),
                                                gPad->GetName());
      if ((func = dynamic_cast<TF1*>(gROOT->FindObject(func_name)))) {
         delete func;
      }

      func = new TF1(func_name,"pol1",
                     gPad->GetUxmin(),gPad->GetUxmax());
      func->SetParameters(y0,a);
      func->Draw("same");
   }
   
   return 1;
}
