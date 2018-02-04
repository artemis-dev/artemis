/**
 * @file   TCmdMpol.cc
 * @brief  polynomial fitting by mouse
 *
 * @date   Created       : 2017-06-14 13:11:48 JST
 *         Last Modified : 2017-06-14 13:12:17 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdMpol.h"

#include <TROOT.h>
#include <TPad.h>
#include <TGraph.h>
#include <TF1.h>
#include <TMarker.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TClonesArray.h>
#include <TCatCmdXval.h>
#include <TCmdComment.h>

using art::TCmdMpol;

ClassImp(TCmdMpol)

const char* TCmdMpol::kFuncNameBase = "f";
const char* TCmdMpol::kLabelNameBase = "l";

TCmdMpol::TCmdMpol()
  : fMarkers(NULL), fMarkerStyle(3)
{
  SetName("mpol");
  SetTitle("polynomial fitting by mouse");
}

TCmdMpol::~TCmdMpol()
{
  delete fMarkers;
}

TCmdMpol* TCmdMpol::Instance()
{
  static TCmdMpol instance;
  return &instance;
}

void TCmdMpol::Help()
{
  printf("usage: %s degree_of_polynomial\n",GetName());
}

Long_t TCmdMpol::Cmd(vector<TString> tokens)
{
  if (tokens.size() != 2) {
    Help();
    return 1;
  }

  if (!tokens[1].IsDec()) {
    Error("Cmd","degree_of_polynomial must be integer.");
    return 1;
  }

   if (gPad == NULL) {
      return 1;
   }
  
  const Int_t degree = tokens[1].Atoi();

  return Run(degree);
}

Long_t TCmdMpol::Run(Int_t degree)
{
  if( degree < 1 || degree > 8) {
    Error("Run","degree_of_polynomial must be "
	        "positive integer less than 9.");
  }

  TGraph *g = new TGraph(degree+1);

  if(!fMarkers) {
    fMarkers = new TClonesArray(TMarker::Class(),9);
  } else {
    fMarkers->Delete();
    gPad->Update();
  }
  Double_t x = 0, y = 0;
  for (Int_t i = 0; i <= degree; ++i) {
    Info("Run","click the point %d/%d",i+1,degree+1);
    TCatCmdXval::Instance()->Run(&x,&y);
    printf("X,Y [%d] = %f, %f\n",i,x,y);
    g->SetPoint(i,x,y);

    TMarker *m = static_cast<TMarker*>(fMarkers->ConstructedAt(i));
    m->SetMarkerStyle(fMarkerStyle);
    m->SetX(x);
    m->SetY(y);
    m->Draw();
    gPad->Update();
  }

  {
     TF1 *func;
     const TString func_name = TString::Format("%s_%s_%s",
                                               kFuncNameBase,
                                               GetName(),
                                               gPad->GetName());
     if ((func = dynamic_cast<TF1*>(gROOT->FindObject(func_name)))) {
        delete func;
     }

     func = new TF1(func_name,TString::Format("pol%d",degree),
                    gPad->GetUxmin(),gPad->GetUxmax());
     g->Fit(func,"0Q");
     func->Draw("same");
     delete g;
     
     TString lstr = TString::Format("[%s]",GetName());

     for (Int_t i = 0; i <= degree; ++i) {
        const TString &s = TString::Format(" a%d = %g,",i,func->GetParameter(i));
        lstr.Append(s);
        printf("a%d = %g\n",i,func->GetParameter(i));
     }

     TCmdComment::Instance()->Run(lstr.Chop());
  }
     
  return 1;
}
