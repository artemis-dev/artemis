/* @file TArtLocalUtil.cc
 * @brief Local utilities
 *
 * @date Create        : 2020-07-28 10:39:15 JST
 *       Last Modified : 2020-08-06 13:51:52 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TArtLocalUtil.h"
#include "TString.h"
#include "TH2.h"
#include "TGraph.h"
#include "TMath.h"
#include "TF1.h"

using namespace art;

TString Util::ZeroFilledDigitForm(int maxNum) {
   int digit = 1;
   while ((maxNum = (maxNum/10))) ++digit;
   return Form("%%0%dd",digit);
}


TH1* Util::ScaleX(TH1* hin, double scale, TH1* hout) {
   if (!hout) {
      hout = (TH1*) hin->Clone("tmp");
   }
   hout->Reset();
   const int nBin = hin->GetNbinsX();
   TAxis *xaxis = hin->GetXaxis();
   for (int ibin = 0; ibin < nBin; ++ibin) {
      const double weight = hin->GetBinContent(ibin+1);
      const double scaledXlow = scale * hin->GetXaxis()->GetBinLowEdge(ibin+1);
      const double scaledXup  = scale * hin->GetXaxis()->GetBinUpEdge(ibin+1);
      const double scaledWidth = scale * hin->GetXaxis()->GetBinWidth(ibin+1);
      const int iBinStart = xaxis->FindBin(scaledXlow);
      const int iBinEnd   = xaxis->FindBin(scaledXup) + 1;
//      printf("%f => %d, %f => %d\n",scaledXlow,iBinStart,scaledXup,iBinEnd);
      for (int ib = iBinStart; ib < iBinEnd; ++ib) {
         const double xlow = std::max(scaledXlow,xaxis->GetBinLowEdge(ib));
         const double xup  = std::min(scaledXup,xaxis->GetBinUpEdge(ib));
         hout->Fill(0.5*(xup+xlow), weight * (xup - xlow)/scaledWidth);
//         printf("xlow = %f xup = %f weight = %f, new weight = %f\n",xlow,xup,weight,weight * (xup - xlow)/scaledWidth);
      }
   }
   return hout;
}


TH2* Util::ShiftCorrectionKT(TH2* h2, std::vector<double>& scales, std::vector<double>& curvature, TH1* href, double kStepMin, double kStepMax, int kNumStep) {
   const char* histname = h2->GetName();
//   const double thres = 100;
   const double kStep = (kStepMax - kStepMin)/(kNumStep - 1);
   if (!h2) return NULL;

   TH2 *h2scaled = (TH2*)h2->Clone(Form("%s_scaled",histname));
   h2scaled->Reset();
   

   const Int_t nBinsX = h2->GetNbinsX();
#if DEBUG   
   gDirectory->mkdir(Form("%s_kt",histname))->cd();
#endif   
   scales.resize(nBinsX,1);
   curvature.resize(nBinsX,0.);
   TGraph *gr = new TGraph(kNumStep);
   
   
   for (Int_t i = 0;  i < nBinsX; ++i) {
      TH1 *h1base = h2->ProjectionY(Form("%s_%03d",histname,i),i+1,i+1);
      TAxis *axis = h1base->GetXaxis();
      if (h1base->Integral(axis->FindBin(1000),axis->GetNbins()) == 0) {
         h1base->Delete();
         continue;
      }
      
      if (!href) {
         href = (TH1*)h1base->Clone("href");         
         for (int ix = 0, nx = href->GetXaxis()->FindBin(1000.); ix < nx; ++ix) {
            href->SetBinContent(ix+1,0.);
         }
      }

      
      // pre-scan
      double minDistance = 0.;
      double minScale = 0.;
      TH1 *h1 = (TH1*)h1base->Clone("tmp");
      std::vector<double> scalexs(kNumStep);
      std::vector<double> rets(kNumStep);
      int minStep = 0;
      for (Int_t iStep = 0; iStep < kNumStep; ++iStep) {
         double scaleX = kStep * iStep + kStepMin;
         Util::ScaleX(h1base,scaleX,h1);
         for (int ix = 0, nx = h1->GetXaxis()->FindBin(1000.); ix < nx; ++ix) {
            h1->SetBinContent(ix+1,0.);
         }
         if (h1->GetEntries() < 10.) {
            scalexs[iStep] = scaleX;
            rets[i] = 1;
            continue;
            
         }
         double ret = h1->KolmogorovTest(href,"");
         scalexs[iStep] = scaleX;
         rets[iStep] = ret;
         if (minDistance < ret) {
            minStep = iStep;
            minDistance = ret;
            minScale = scaleX;
         }
//         printf("scale %f ret %f\n",scaleX,ret);
      }

      int begin = TMath::Max(minStep-3,0);
      int end = TMath::Min(begin + 6, kNumStep);
      if (end == kNumStep) {
         begin = end - 6;
      }
      gr->Set(6);
      for (int istep = begin; istep < end; ++istep) {
         gr->SetPoint(istep - begin, scalexs[istep],rets[istep]);
      }
      gr->Fit("pol2","Q");
//      gr->Draw("AP*");
      double a = gr->GetFunction("pol2")->GetParameter(2);
      double b = gr->GetFunction("pol2")->GetParameter(1);
      double scale = -b*0.5/a;
//      printf ("scale (pre) = %f\n",scale);
      
      const double newStepMin = scale - kStep;
      const double newStepMax = scale + kStep;
      const double newStep = (newStepMax - newStepMin) / (kNumStep - 1);
      minDistance = 0;
      
      for (Int_t iStep = 0; iStep < kNumStep; ++iStep) {
         double scaleX = newStep * iStep + newStepMin;
         Util::ScaleX(h1base,scaleX,h1);
         for (int ix = 0, nx = h1->GetXaxis()->FindBin(1000.); ix < nx; ++ix) {
            h1->SetBinContent(ix+1,0.);
         }
         if (h1->GetEntries() < 10.) {
            scalexs[iStep] = scaleX;
            rets[i] = 1;
            continue;
            
         }
         double ret = h1->KolmogorovTest(href,"");
         scalexs[iStep] = scaleX;
         rets[iStep] = ret;
         if (minDistance < ret) {
            minStep = iStep;
            minDistance = ret;
         }
//         printf("scale %f ret %f\n",scaleX,ret);
      }
//      printf("minStep = %d\n",minStep);
      
      delete h1;
      begin = TMath::Max(minStep - 3,0);
      end = TMath::Min(begin + 6, kNumStep);
      if (end == kNumStep) begin = end - 6;
      gr->Set(6);
      for (int istep = begin; istep < end; ++istep) {
//         printf("using %d scalex = %f, ret = %f\n",istep,scalexs[istep],rets[istep]);
         gr->SetPoint(istep - begin, scalexs[istep],rets[istep]);
      }
      gr->Fit("pol2","Q");
//      gr->Draw("AP*");
      a = gr->GetFunction("pol2")->GetParameter(2);
      b = gr->GetFunction("pol2")->GetParameter(1);
      scale = -b*0.5/a;
//      printf("scale = %f\n",scale);
      
      scales[i] = scale;
      
      curvature[i] = a;
      delete h1base;
   }
   delete gr;

   if (href) delete href;
   

   for (int ix = 0; ix < nBinsX; ++ix) {
      TH1* h1 = (TH1*) h2->ProjectionY("tmp",ix+1,ix+1);
      TH1* h1s = Util::ScaleX(h1,scales[ix]);
      for (int iy = 0, nBinsY = h2->GetNbinsY(); iy < nBinsY; ++iy ) {
         h2scaled->SetBinContent(ix+1,iy+1,h1s->GetBinContent(iy+1));
      }
      delete h1;
      delete h1s;
   }

   return h2scaled;
   
}



