/**
 * @file   TCatRefineRecoilClusterProcessor.cc
 * @brief  refine recoil hit cluster
 *
 * @date   Created       : 2015-05-14 13:59:54 JST
 *         Last Modified : 2015-08-31 17:15:24 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatRefineRecoilClusterProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TGraph.h>
#include <TF1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TSimpleData.h>
#include <numeric>
void Robust(Int_t n, Double_t *x, Double_t *y, Double_t &p0, Double_t &p1, Double_t s, Int_t nIter = 10);
void Robust2(Int_t n, Double_t *xin, Double_t *yin, Double_t *zin,Double_t *charge,
             Double_t &p0, Double_t &p1, Double_t &pz0, Double_t &pz1, Double_t sss, Int_t nIter=10);
using art::TCatRefineRecoilClusterProcessor;

ClassImp(TCatRefineRecoilClusterProcessor)
TCatRefineRecoilClusterProcessor::TCatRefineRecoilClusterProcessor()
: fInputName(""), fInput(NULL), fOutputName(""), fOutput(NULL)
{
   RegisterInputCollection("InputCollection","name of input collection",fInputName,TString("atroughrecoil"),
                           &fInput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputCollection","name of output collection",fOutputName,TString("atrecoil"),
                            &fOutput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputDistance","name of output distance",fOutputNameDistance,TString("distance"),
                            &fDistance,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("OutputLength","name of output distance",fOutputNameLength,TString("length"),
                            &fLength,TSimpleData::Class_Name());

   RegisterProcessorParameter("RequiredNumHits","the number of hits required to continue analysis (default: 3)",
                              fRequredNumHits,3);
}

TCatRefineRecoilClusterProcessor::~TCatRefineRecoilClusterProcessor()
{
}

TCatRefineRecoilClusterProcessor::TCatRefineRecoilClusterProcessor(const TCatRefineRecoilClusterProcessor& rhs)
{
}

TCatRefineRecoilClusterProcessor& TCatRefineRecoilClusterProcessor::operator=(const TCatRefineRecoilClusterProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TCatRefineRecoilClusterProcessor::Process()
{
   static TGraph graph;
   TClonesArray *input = (*fInput);
   fOutput->Clear("C");
   fDistance->Clear("C");
   fLength->Clear("C");
   Int_t nHit = input->GetEntries();
   if (!nHit) return;

   graph.Set(nHit);
   Double_t xmin = 1e6;
   Double_t xmax = -1e6;
   Double_t xminy, xmaxy;
   Info("Process","entry");
   std::vector<Double_t> xhit,yhit,zhit;
   std::vector<Double_t> charge;
   std::vector<TCatPulseShape*> hitpulse;
   for (Int_t iHit = 0; iHit != nHit; ++iHit) {
      TCatPulseShape *pulse = (TCatPulseShape*) input->UncheckedAt(iHit);
      Double_t x = pulse->GetX();
      Double_t z = pulse->GetZ();
      Double_t y = pulse->GetOffset()*0.02 * 10.;
      if (TMath::Abs(x) < 11.) continue;
      hitpulse.push_back(pulse);
      xhit.push_back(x);
      yhit.push_back(y);
      zhit.push_back(z);
      charge.push_back(pulse->GetCharge());
      graph.SetPoint(iHit,x,y);
   }
   nHit = xhit.size();
#if 0  
   graph.Fit("pol1","ROB");
   Info("Process","fit done");
   TF1* func = graph.GetFunction("pol1");
   Double_t p0 = func->GetParameter(0);
   Double_t p1 = func->GetParameter(1);
#else
   Double_t p0;
   Double_t p1;
   Double_t pz0;
   Double_t pz1;
//   Robust(nHit,graph->GetX(),graph->GetY(),p0,p1,5,10);
   Robust2(nHit,&xhit[0],&yhit[0],&zhit[0],&charge[0],p0,p1,pz0,pz1,5,10);
#endif
   
   Info("Process","param done p0 = %f p1 = %f",p0,p1);
  
   for (Int_t iHit = 0; iHit != nHit; ++iHit) {
      // y = p0 + p1 * x <=> p1 * x - y + p0 = 0
      TCatPulseShape *pulse = hitpulse[iHit];
      Double_t d = (p1 * xhit[iHit]  - yhit[iHit] + p0) / TMath::Sqrt(p1*p1+1);
      TSimpleData *data = (TSimpleData*) fDistance->ConstructedAt(fDistance->GetEntriesFast());
      data->SetValue(d);
      if (TMath::Abs(d) < 5.) {
         TCatPulseShape *outpulse = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
         pulse->Copy(*outpulse);
      }
   }
   nHit = fOutput->GetEntries();
   graph.Set(nHit);
   for (Int_t iHit = 0; iHit != nHit; ++iHit) {
      TCatPulseShape *pulse = (TCatPulseShape*) fOutput->UncheckedAt(iHit);
      Double_t x = pulse->GetX();
      Double_t y = pulse->GetOffset()*0.02 * 10.;
      graph.SetPoint(iHit,x,y);
      if (xmin > x) {
         xmin = x;
         xminy = y;
      }
      if (xmax < x) {
         xmax = x;
         xmaxy = y;
      }
   }
   fLength->SetValue(TMath::Sqrt((xmax - xmin) * (xmax - xmin)));
   fLength->Validate();
   if (fLength->GetValue() > 50) {
      fOutput->Clear("C");
      return;
   }
   Info("Process","check length done. nHit = %d",nHit);
   Info("Process","done");
   
}


void Robust(Int_t n, Double_t *xin, Double_t *yin, 
            Double_t &p0, Double_t &p1,  Double_t sss, Int_t nIter)
{
   using std::accumulate;
   using std::inner_product;
   std::vector<Double_t> x(xin,xin+n);
   std::vector<Double_t> y(yin,yin+n);
   std::vector<Double_t> wxx(n);;
   std::vector<Double_t> wxy(n);;
   std::vector<Double_t> wx(n);;
   std::vector<Double_t> wy(n);;

   std::vector<Double_t> w(n);;

   Double_t xsum = accumulate(x.begin(),x.end(),0.);
   Double_t ysum = accumulate(y.begin(),y.end(),0.);

   Double_t xxsum = inner_product(x.begin(),x.end(),x.begin(),0.);
   Double_t xysum = inner_product(x.begin(),x.end(),y.begin(),0.);

   p0 = (xxsum * ysum - xysum * xsum) / (n * xxsum - xsum * xsum);
   p1 = (n * xysum - xsum * ysum) / (n * xxsum - xsum * xsum);

   printf("p0 = %f p1 = %f\n",p0,p1);

   // p1 * x - y + p0 = 0
   for (Int_t iIter = 0; iIter != nIter; ++iIter) {
      Double_t s = 0.;
      for (Int_t i=0; i!=n; ++i) {
         Double_t X = x[i];
         Double_t Y = y[i];
         Double_t d = TMath::Abs((p1 * X - Y + p0) / TMath::Sqrt(p1 * p1 + 1));
         s += 2*d;
      }
      s /= n;
      for (Int_t i=0; i!=n; ++i) {
         Double_t X = x[i];
         Double_t Y = y[i];
         Double_t d = TMath::Abs((p1 * X - Y + p0) / TMath::Sqrt(p1 * p1 + 1));
         w[i] = d > s ? 0 : (1-(d/s)*(d/s)) * (1-(d/s)*(d/s));
         wx[i] = w[i] * X;
         wy[i] = w[i] * Y;
         wxx[i] = w[i] * X * X;
         wxy[i] = w[i] * X * Y;
      }
      Double_t wxsum = accumulate(wx.begin(),wx.end(),0.);
      Double_t wysum = accumulate(wxy.begin(),wxy.end(),0.);
      Double_t wxxsum = accumulate(wxx.begin(),wxx.end(),0.);
      Double_t wxysum = accumulate(wxy.begin(),wxy.end(),0.);
      Double_t wsum = accumulate(w.begin(),w.end(),0.);
      p0 = (wxxsum * wysum - wxysum * wxsum) / (wsum * wxxsum - wxsum * wxsum);
      p1 = (wsum * wxysum - wxsum * wysum) / (wsum * wxxsum - wxsum * wxsum);
   }
   return;
}

#if 1
void Robust2(Int_t n, Double_t *xin, Double_t *yin, Double_t *zin,Double_t *charge,
            Double_t &p0, Double_t &p1, Double_t &pz0, Double_t &pz1, Double_t sss, Int_t nIter)
{
   using std::accumulate;
   using std::inner_product;
   std::vector<Double_t> x(xin,xin+n);
   std::vector<Double_t> y(yin,yin+n);
   std::vector<Double_t> z(zin,zin+n);
   std::vector<Double_t> wxx(n);;
   std::vector<Double_t> wxy(n);;
   std::vector<Double_t> wxz(n);;
   std::vector<Double_t> wx(n);;
   std::vector<Double_t> wy(n);;
   std::vector<Double_t> wz(n);;
   std::vector<Double_t> w(n);;
   std::vector<Double_t> w1(n);;

   Double_t xsum = accumulate(x.begin(),x.end(),0.);
   Double_t ysum = accumulate(y.begin(),y.end(),0.);
   Double_t zsum = accumulate(z.begin(),z.end(),0.);
   Double_t xxsum = inner_product(x.begin(),x.end(),x.begin(),0.);
   Double_t xysum = inner_product(x.begin(),x.end(),y.begin(),0.);
   Double_t xzsum = inner_product(x.begin(),x.end(),z.begin(),0.);
   
   p0 = (xxsum * ysum - xysum * xsum) / (n * xxsum - xsum * xsum);
   p1 = (n * xysum - xsum * ysum) / (n * xxsum - xsum * xsum);
   pz0 = (xxsum * zsum - xzsum * xsum) / (n * xxsum - xsum * xsum);
   pz1 = (n * xzsum - xsum * zsum) / (n * xxsum - xsum * xsum);

   printf("p0 = %f p1 = %f\n",p0,p1);

   // p1 * x - y + p0 = 0
   for (Int_t iIter = 0; iIter != nIter; ++iIter) {
      Double_t s = 0.;
      Double_t sz = 0.;
      Double_t sc = 0.;
      for (Int_t i=0; i!=n; ++i) {
         Double_t X = x[i];
         Double_t Y = y[i];
         Double_t Z = z[i];
         Double_t d = TMath::Abs((p1 * X - Y + p0) / TMath::Sqrt(p1 * p1 + 1));
         Double_t dz = TMath::Abs((pz1 * X - Z + pz0) / TMath::Sqrt(pz1 * pz1 + 1));
         sc = sc < charge[i] ? charge[i] : sc;
         s += 2*d;
         sz += 2*dz;
      }
      s /= n;
      sz /= n;
      for (Int_t i=0; i!=n; ++i) {
         Double_t X = x[i];
         Double_t Y = y[i];
         Double_t Z = z[i];
         Double_t d = TMath::Abs((p1 * X - Y + p0) / TMath::Sqrt(p1 * p1 + 1));
         Double_t dz = TMath::Abs((pz1 * X - Z + pz0) / TMath::Sqrt(pz1 * pz1 + 1));
         w[i] = d > s ? 0 : (1-(d/s)*(d/s)) * (1-(d/s)*(d/s));
         w1[i] = dz > sz ? 0 : (1-(dz/sz)*(dz/sz)) * (1-(dz/sz)*(dz/sz));
         w[i] *= (1-(charge[i]/sz*charge[i]/sz)) * (1-(charge[i]/sz*charge[i]/sz));
         wx[i]  = w1[i] * w[i] * X;
         wy[i]  = w1[i] * w[i] * Y;
         wz[i]  = w1[i] * w[i] * Z;
         wxx[i] = w1[i] * w[i] * X * X;
         wxy[i] = w1[i] * w[i] * X * Y;
         wxz[i] = w1[i] * w[i] * X * Z;
      }
      Double_t wxsum = accumulate(wx.begin(),wx.end(),0.);
      Double_t wysum = accumulate(wy.begin(),wy.end(),0.);
      Double_t wzsum = accumulate(wz.begin(),wz.end(),0.);
      Double_t wxxsum = accumulate(wxx.begin(),wxx.end(),0.);
      Double_t wxysum = accumulate(wxy.begin(),wxy.end(),0.);
      Double_t wxzsum = accumulate(wxz.begin(),wxz.end(),0.);
      Double_t wsum = inner_product(w.begin(),w.end(),w1.begin(),0.);
//      Double_t w1sum = accumulate(w1.begin(),w1.end(),0.);
      p0 = (wxxsum * wysum - wxysum * wxsum) / (wsum * wxxsum - wxsum * wxsum);
      p1 = (wsum * wxysum - wxsum * wysum) / (wsum * wxxsum - wxsum * wxsum);
      pz0 = (wxxsum * wzsum - wxzsum * wxsum) / (wsum * wxxsum - wxsum * wxsum);
      pz1 = (wsum * wxzsum - wxsum * wzsum) / (wsum * wxxsum - wxsum * wxsum);
   }
   return;
}
#endif
