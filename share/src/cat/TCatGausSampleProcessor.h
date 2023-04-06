/* $Id:$ */
/**
 * @file   TCatGausSampleProcessor.h
 * @date   Created : Mar 11, 2013 05:11:23 JST
 *   Last Modified : Mar 20, 2013 04:21:06 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATGAUSSAMPLEPROCESSOR_H
#define TCATGAUSSAMPLEPROCESSOR_H

#include <TProcessor.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH2F.h>
#include <Fit/Fitter.h>
#include <TMinuit.h>
#include <vector>
using namespace std;


namespace art {
  class TCategorizedData;
  class TCatGausSampleProcessor;
}

class art::TCatGausSampleProcessor  : public art::TProcessor {

public:
   TCatGausSampleProcessor();
   ~TCatGausSampleProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();

protected:
   class FCN {
   public:
      FCN() {;}
      ~FCN() {;}
      static void calcfcn(Int_t &nPar, Double_t *gin, Double_t &fcn, Double_t *par, Int_t flag) {
         fcn = 0.;
         double siginv = 1./par[2];
         for (Int_t i=0; i!=fN; i++) {
            double x = (fX[i]-par[1])*siginv;
            double xx = x*x*0.5;
            double diff = fY[i] - par[0] * exp(-xx);
            fcn += diff*diff;
         }
      }
         

      inline double operator() (const double* par) {
         fFCN = 0.;
         double siginv = 1./par[2];
         for (Int_t i=0; i!=fN; i++) {
            double x = (fX[i]-par[1])*siginv;
            double xx = x*x*0.5;
            double diff = fY[i] - par[0] * exp(-xx);
            fFCN += diff*diff;
         }
         return fFCN;
      }
      void Add(double x, double y) {
         fX.push_back(x);
         fY.push_back(y);
         fN++;
      }
      void Clear() {
         fN = 0;
         fX.clear();
         fY.clear();
      }
   private:
      static vector<double> fX;
      static vector<double> fY;
      static Int_t fN;
      static double fFCN;
   };

protected:
   TCategorizedData **fCategorized; //!
   TClonesArray          *fCATHit;
   TClonesArray       *fCATHitOut;

   StringVec_t  fInputColName;
   TString      fOutputColName;
   Int_t        fIsSparse; 
   TGraph      *fGraph;
   TF1         *fGaus;
   FCN          fFCN;
   ROOT::Fit::Fitter      fFitter;
   Int_t        fNPar;
   Double_t     fPar[3];
   Float_t     fSigma;
   TMinuit     fMinuit;
   TH2F        *fHistMean;
   TH2F        *fHistSigma;
   TH2F        *fHistConst;
   TH1F        *fHistFitterFit;
   TH1F        *fHistMinuitFit;

   ClassDef(TCatGausSampleProcessor,1)
};
#endif // end of #ifdef TCATGAUSSAMPLEPROCESSOR_H
