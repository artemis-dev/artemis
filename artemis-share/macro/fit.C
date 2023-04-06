/* $Id:$ */
/**
 * @file   fit.C
 * @date   Created : Mar 18, 2013 17:18:52 JST
 *   Last Modified : Mar 19, 2013 01:45:48 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */

#include <Fit/Fitter.h>
#include <Fit/Chi2FCN.h>
#include <TF1.h>
#include <TStopwatch.h>
#include <TGraph.h>
#include <iostream>

class MyFCN
{
public:
   MyFCN() 
      : fFCN(0.)
   {
      fun = new TF1("fun","[0]*exp(-(x/[1])*(x/[1])/2.)",-100,100);
      fun->SetParameter(0,10.);
      fun->FixParameter(1,6.30);
//      printf("%f\n",fun->Eval(0));
//      printf("%f\n",fun->Eval(10));
//      printf("%f\n",fun->Eval(20));
      fN = 0;
   }
   double operator() (const double* par) {
      fFCN = 0.;
//      fun->SetParameter(0,par[0]);
      Double_t p0 = par[0];
      Double_t p1 = par[1];
      Double_t sinv = 1./6.30;
      for (Int_t i=0;i!=fN; i++) {
         double x = (fX[i]-p1)*sinv;
         double x2 = x*x*0.5;
         double diff = fY[i] - p0 * exp (-x2);
         fFCN += diff*diff;
      }
      return fFCN;
   }
   void Add(double x, double y) { 
      fX[fN] = x;
      fY[fN] = y;
      fN++;
   }
   void Clear() { fN = 0; }
private:
   TF1 *fun;
   double fY[35];
   double fX[35];
   Int_t fN;
   double fFCN;
};


void fit()
{
   double y[] = { -3.299999,-5.300000,1.7000000,-7.300000,3.7000000,
           -1.299999,-7.300000,1.7000000,7.6999998,6.6999998,
           18.700000,28.700000,31.700000,41.700000,44.700000,
           40.700000,44.700000,51.700000,46.700000,45.700000,
           39.700000,29.700000,24.700000,23.700000,14.699999,
           11.699999,11.699999,2.7000000,-3.299999,3.7000000,
           -2.299999,1.7000000,-4.300000,-13.30000,-7.300000};
   double x[] ={ 8, 9, 11, 12, 14, 
             15, 16, 17, 18, 20, 
             21, 23, 24, 25, 26, 
             27, 28, 29, 30, 32, 
             33, 35, 36, 38, 39, 
             40, 41, 42, 44, 45, 
             47, 48, 49, 50, 51 };

   MyFCN myFCN;
   for (Int_t i=0; i!= 35; i++) {
      myFCN.Add(x[i],y[i]);
   }
   const int nPar = 2;
   double par[nPar] = {10.,30};
   ROOT::Fit::Fitter fitter;
   fitter.Config().SetParamsSettings(nPar,par);
   TGraph graph(35,x,y);
   TF1 *gaus = new TF1("gaus","gaus",-100.,100);
   gaus->SetParameters(10,30);
   gaus->FixParameter(2,6.3);
   TStopwatch stopwatch;
   stopwatch.Start();
   fitter.FitFCN(nPar,myFCN,par,35,true);
   stopwatch.Stop();
   printf("real %g sec\n",stopwatch.RealTime());
   ROOT::Fit::FitResult result = fitter.Result();
   result.Print(std::cout);
   stopwatch.Start();
   graph.Fit(gaus,"QN");
   stopwatch.Stop();
   printf("real %g sec\n",stopwatch.RealTime());
   
}





