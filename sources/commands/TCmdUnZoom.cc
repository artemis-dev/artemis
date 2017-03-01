/**
 * @file   TCmdUnZoom.cc
 * @brief  unzoom current histogram
 *
 * @date   Created       : 2015-12-21 13:30:00 JST
 *         Last Modified : 2015-12-22 13:33:01 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TCmdUnZoom.h"

#include <TROOT.h>
#include <TH1.h>
#include <TPad.h>
#include <TAxis.h>
#include <TRegexp.h>

using art::TCmdUnZoom;

ClassImp(TCmdUnZoom)

TCmdUnZoom::TCmdUnZoom()
{
   SetName("unzoom");
   SetTitle("unzoom current historgram");
}

TCmdUnZoom::~TCmdUnZoom()
{
}

Long_t TCmdUnZoom::Cmd(vector<TString> tokens)
{
   const TString default_opt = "xyz";
   const TString option =
      tokens.size() > 1 && tokens[1].Contains(TRegexp("[xyz]")) ? tokens[1] : default_opt;

   TPad *const currentPad = static_cast<TPad*>(gROOT->GetSelectedPad());
   if (!currentPad) return 1;

   TIter next(currentPad->GetListOfPrimitives());
   while (TObject *const obj = next()) {
      if (obj->InheritsFrom("TH1")) {
	 Run(dynamic_cast<TH1*>(obj),option);
	 currentPad->Modified();
	 currentPad->Update();
	 return 1;
      }
   }
   return 1;
}

Long_t TCmdUnZoom::Run(TH1* h, Option_t *opt = "xyz") const
{
   TString option = opt;
   option.ToLower();
   switch (const Int_t nDim = h->GetDimension()){
      case 3:
	 if (option.Contains("z")) h->GetZaxis()->UnZoom();
	 /* fall through */
      case 2:
	 if (option.Contains("y")) h->GetYaxis()->UnZoom();
	 /* fall through */
      case 1:
	 if (option.Contains("x")) h->GetXaxis()->UnZoom();
	 break;
      default:
	 printf("%s: unexpected dimension of the histogram (nDim = %d)\n",
		GetName(),nDim);
	 return 1;
   }
   return 1;
}

void TCmdUnZoom::Help()
{
   printf("Usage   : %s [xyz]\n",GetName());
}
