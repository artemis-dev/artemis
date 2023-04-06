/* $Id:$ */
/**
 * @file   CalcPedThres.C
 * @date   Created : Sep 19, 2013 14:19:59 JST
 *   Last Modified : Sep 19, 2013 15:13:09 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef __CINT__
#include <TFile.h>
#include <TH2.h>
#include <fstream>
#include <TProfile.h>
#endif

void CalcPedThres(const char* filename, const char* histname, const char* outputname, double range)
{
   TFile *file = TFile::Open(filename);
   if (!file) {
      printf("No such file %s\n",filename);
      return;
   }
   TH2 *hist = (TH2*) file->Get(histname);
   if (!hist) {
      printf("No such hist %s\n",histname);
      return;
   }
   std::ofstream fout;
   fout.open(outputname);
   if (!fout) {
      printf("Cannot open file %s\n",outputname);
      return;
   }

   Int_t bin = hist->GetMaximumBin();
   Int_t xbin = -1;
   Int_t ybin = -1;
   Int_t zbin = -1;
   hist->GetBinXYZ(bin,xbin,ybin,zbin);
   Double_t ymean = hist->GetYaxis()->GetBinCenter(ybin);
   hist->GetYaxis()->SetRangeUser(ymean-range,ymean+range);
   Int_t nbinsx = hist->GetNbinsX();
   TH2F *hthres = new TH2F("hthres","threshold",nbinsx,-0.5,nbinsx-0.5,2*range,ymean-range,ymean+range);
   double maxThres = 0;
   for (int ibin = 0; ibin!=nbinsx; ibin++) {
      bin = ibin +1;
      TH1* h = hist->ProjectionY("htemp",bin,bin);
      Double_t mean = h->GetMean();
      Double_t err  = h->GetRMS();
      Double_t thres = mean + 5 * err;
      if (maxThres < thres) maxThres = thres;
      if (((ibin+1)%8)==0) {
         fout << (ibin / 64) << " " << ((ibin/8)%8) << " " << maxThres << " " << mean << " " << err << endl;
         for (Int_t i = 0; i!=8;i++) {
            hthres->Fill(ibin-i,maxThres);
         }
         maxThres = 0;
      }
      h->Delete();
   }
   
   fout.close();
//   file->Close();
}
