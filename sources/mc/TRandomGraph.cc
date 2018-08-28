/**
 * @file   TRandomGraph.cc
 * @brief
 *
 * @date   Created       : 2015-12-21 15:26:59 JST
 *         Last Modified : 2018-08-27 17:47:53 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TRandomGraph.h"
#include <TSpline.h>

using art::TRandomGraph;

ClassImp(TRandomGraph)

art::TRandomGraph::TRandomGraph(TGraph *graph)
   : TGraph(*graph)
{
   Init();
}


art::TRandomGraph::TRandomGraph(const char* filename, const char* format)
   : TGraph(filename,format)
{
   Init();
}


art::TRandomGraph::~TRandomGraph()
{
}


Double_t art::TRandomGraph::Generate(Double_t x1, Double_t x2)
{
   while (1) {
      Double_t t = gRandom->Uniform(x1,x2);
      Double_t y = gRandom->Uniform(0,fMaximum);
      if (fSpline->Eval(t) > y) {
         return t;
      }
   }
}


void art::TRandomGraph::Init()
{
   fMaximum = 0;
   fSpline = new TSpline3("spline",GetX(),GetY(),GetN());
   for (Int_t i=0; i!= fNpoints; ++i) {
      if (fY[i] > fMaximum) {
         fMaximum = fY[i];
      }
   }
}

Double_t art::TRandomGraph::Integral(Double_t x1, Double_t x2, Double_t step) const
{
   Double_t x = x1;
   Double_t sum = 0.;
   while (x < x2) {
      sum += (fSpline->Eval(x)+fSpline->Eval(x+step))* 0.5 * step;
      x += step;
   }
   return sum;
}

