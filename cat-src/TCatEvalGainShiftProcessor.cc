/* @file TCatEvalGainShiftProcessor.cc
 * @brief evaluation of gain shift and obtain correction parameters
 *
 * @date Create        : 2020-07-27 14:08:51 JST
 *       Last Modified : 2020-08-22 10:53:48 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 * This processor diagnoses the status of each readout pad and
 * extracts gain shift correction parameters. The option of the status is bad pads 
 *
 *
 */


#include "TCatEvalGainShiftProcessor.h"
#include "TChain.h"
#include "TMath.h"
#include "TArtLocalUtil.h"
#include "TH2F.h"
#include "TEventHeader.h"
#include "TCatTpcTrack.h"
#include "TFile.h"
#include "TROOT.h"
#include "TCatReadoutPadArray.h"
#include "TCatReadoutPad.h"
#include "TH2Poly.h"
#include <numeric>
#include <fstream>
#include "TRunInfo.h"

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


ClassImp(art::TCatEvalGainShiftProcessor)

using art::TCatEvalGainShiftProcessor;

#ifdef USE_MPI
namespace art {
   namespace mpi {
      void GetMPISizeRankIfInitialized(int &useMPI, int &npe, int &myrank) {
         useMPI = 0;
         npe = 0;
         myrank = 0;
         MPI_Initialized(&useMPI);
         if (useMPI) {
            MPI_Comm_size(MPI_COMM_WORLD, &npe);
            MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
         }
      }
   }
};
#endif         


TCatEvalGainShiftProcessor::TCatEvalGainShiftProcessor()
{
   Register(fInputFilename("InputFile","name of input tree file","path/to/input.tree.root"));
   Register(fOutputFilename("OutputFile","name of output file","path/to/output.hist.root"));
   Register(fEventsModulus("EventsModulus","modulus of the number of events",1000.));
   Register(fEventHeaderName("EventHeader","name of event header","eventheader"));
   Register(fCorrectionFilename("CorrectionFile","output correction parameters","path/to/correction.dat"));
   Register(fChargeRange("ChargeRange","Axis range of charge [nbins, min, max]",DoubleVec_t(3,0.)));
   Register(fChargeThreshold("ChargeThreshold","Threshold for chargeRange",1000.));
   Register(fReadoutPadName("ReadoutPad","name of readout pad","readoutPad"));
   Register(fInputTrackName("InputTrack","name of input track","atcluster"));
   Register(fCountRatioThreshold("CountRatioThreshold","Threshold for count ratio",0.1));
   Register(fCountThreshold("CountThreshold","Threshold for count",100));
   Register(fDeviationThreshold("DeviationThreshold","Threshold for deviation of gain shift",5.));
}

TCatEvalGainShiftProcessor::~TCatEvalGainShiftProcessor()
{
}


void TCatEvalGainShiftProcessor::EndOfRun()
{
   // input root file
   const TString& inputFilename = fInputFilename;
   const TString& outputFilename = fOutputFilename;
   if (outputFilename.IsNull()) return;
   TString filename = outputFilename;
   
#ifdef USE_MPI   
   int myrank, npe,useMPI;
   mpi::GetMPISizeRankIfInitialized(useMPI,npe,myrank);
   if (useMPI) filename = Form("%s%d",filename.Data(),myrank);
#endif   

   if (TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(inputFilename)) {
      TAnalysisInfo::RemoveFrom(file);
      file->Close();
   }

#ifdef USE_MPI
   if (useMPI) MPI_Barrier(MPI_COMM_WORLD);
   if (myrank) return;
#endif

   // prepare for chain
   TChain *chain = new TChain("tree");
   TClonesArray *track = NULL;
   TEventHeader *header = NULL;
   chain->Add(Form("%s*",inputFilename.Data()));
   chain->SetBranchAddress(fInputTrackName.Value(),&track);
   chain->SetBranchAddress((TString)fEventHeaderName,&header);
   chain->GetEntry(0);
   chain->SetBranchStatus("*",0);
   chain->SetBranchStatus(fInputTrackName.Value(),1);
   chain->SetBranchStatus((TString)fEventHeaderName,1);
   TString runName = header->GetRunName();
   Int_t runNumber = header->GetRunNumber();
   

   TTree *tree = chain->GetTree();
   TCatReadoutPadArray* readoutPad = static_cast<TCatReadoutPadArray*>(tree->GetUserInfo()->FindObject(fReadoutPadName.Value()));
   if (!readoutPad) {
      Error(__func__,"Error no readout pad is found named '%s'\n",fReadoutPadName.Value().Data());
      return;
   }

   const char* runHeaderName = "runheader";
   TList *runInfoList = (TList*)tree->GetUserInfo()->FindObject(runHeaderName);
   if (!runInfoList) {
      Error(__func__,"Could not find run header named '%s",runHeaderName);
      return;
   }
   TRunInfo *runInfo = (TRunInfo*)runInfoList->At(0);
   const int maxEventNumber = runInfo->GetEventNumber();
   
   
   ////////////////////////////////////////////////////////////////////////
   //
   // prepare output directories and histograms for diagnosis and correction results
   //
   ////////////////////////////////////////////////////////////////////////
   
   TFile *file = TFile::Open(outputFilename,"recreate");
   TAnalysisInfo::AddTo(file);

   // prepare for histogram
   const int nEntries = chain->GetEntries();
   const int nBins = TMath::Max(1,TMath::FloorNint(maxEventNumber/fEventsModulus));
   const int nPads = readoutPad->GetEntries();
   std::vector< std::vector<Double_t> > scalesArray(nPads,std::vector<Double_t>(nBins,1.));
   std::vector<Int_t> eventBorders(nBins);
   std::vector<TH2*> fHists(nPads);
   std::vector<TH2*> corrhists(nPads);
   std::vector<TH2*> diaghists(nPads);
   TString idfmt = Util::ZeroFilledDigitForm(nPads);
   TString hnamefmt = Form("h%s",idfmt.Data());
   TString dnamefmt = Form("d%s",idfmt.Data());
   
   TString htitlefmt = Form("charge vs evtnum (%s)",idfmt.Data ());

   TDirectory *topdir = file->mkdir(GetName());
   TDirectory *histdir = topdir->mkdir("TimeEvolution");
   TDirectory *corrdir = topdir->mkdir("Corrected");
   TDirectory *corrdirShift = topdir->mkdir("CorrectedShift");
   TDirectory *shiftdir = topdir->mkdir("ShiftedPads");
   TDirectory *baddir = topdir->mkdir("BadPads");
   TDirectory *diagdir = topdir->mkdir("Diagnosis");
   
   
   topdir->cd();
   TH2* hDiag = new TH2F("hDiag","shift vs id (diag)",nPads,-0.5,nPads-0.5,100,0.0,3.);
   TH2* hEval = new TH2F("hEval","shift vs id",nPads,-0.5,nPads-0.5,100,0.,2.);
   
   TH2Poly *hid  = readoutPad->CreateHist("hIDs","id",-50.,50.,-50.,50.,1);
   TH2Poly *hMean = readoutPad->CreateHist("hMean","average charge",-50.,50.,-50.,50.);
   TH2Poly *hHits = readoutPad->CreateHist("hHits","count",-50.,50.,-50.,50.);
   TH2Poly *hHitsOverThres = readoutPad->CreateHist("hHitsOverThres","count over threshold",-50.,50.,-50.,50.);
   TH2Poly *hHitsRatio = readoutPad->CreateHist("hHitsRatio","ratio over threshold",-50.,50.,-50.,50.);
   TH2Poly *hMaxDeviation = readoutPad->CreateHist("hMaxDeviation","max deviation of shift correction",-50.,50.,-50.,50.);
   TH2Poly *hMaybeBad = readoutPad->CreateHist("hMaybeBad","Candidate bad pads",-50.,50.,-50.,50.);
   TH2Poly *hMayHaveShift = readoutPad->CreateHist("hMayHaveShift","Candidate gain shifted pads",-50.,50.,-50.,50.);
   
   topdir->Add(hid);
   topdir->Add(hHits);
   topdir->Add(hHitsOverThres);
   topdir->Add(hHitsRatio);
   topdir->Add(hMean);
   topdir->Add(hMaxDeviation);
   topdir->Add(hMaybeBad);
   topdir->Add(hMayHaveShift);
   
   histdir->cd();
   const DoubleVec_t& rangeYaxis = static_cast<DoubleVec_t>(fChargeRange);
   for (int i = 0; i < nPads; ++i) {
      const TString hname = Form(hnamefmt,i);
      const TString htitle = Form(htitlefmt,i);
      const int ndiag = nBins - 5 + 1;
      diaghists[i] = new TH2F(hname,htitle,ndiag,-0.5,ndiag-0.5,ndiag,-0.5,ndiag-0.5);
      diaghists[i]->SetDirectory(diagdir);
      
      fHists[i] = new TH2F(hname,htitle,nBins,0.,maxEventNumber,rangeYaxis[0],rangeYaxis[1],rangeYaxis[2]);
   }


   // loop over pads
   for (int ievt = 0; ievt < nEntries; ++ievt) {
      chain->GetEntry(ievt);
      if (!track || !header) continue;
      const int evtid = header->GetEventNumber();
      const int nTracks = track->GetEntriesFast();
      for (int itrack = 0; itrack < nTracks; ++itrack) {
         TCatTpcTrack *aTrack = static_cast<TCatTpcTrack*>(track->UncheckedAt(itrack));
         std::vector<TCatTrackResultPulse*> hits = aTrack->GetHits();
         const int nHits = hits.size();
         if (nHits > 200) continue;
         for (int ihit = 0; ihit < nHits; ++ihit) {
            fHists[hits[ihit]->GetID()]->Fill(evtid, hits[ihit]->GetCharge());
         }
      }
   }
   const double binw = fHists[0]->GetXaxis()->GetBinWidth(1);
   for (int ibin = 0; ibin < nBins; ++ibin) {
      eventBorders[ibin] = TMath::CeilNint(binw * (ibin));
   }

   // diagnosis of the number of counts
   std::vector<bool>  isBad(nPads,false);
   std::vector<bool> hasGainShift(nPads,false);
   std::vector<bool> lowCharge(nPads,false);
   std::vector<double> qmean(nPads);
   double qave = 0.;
   int qnum = 0;
   for (int ipad = 0; ipad < nPads; ++ipad){
      TAxis *yaxis = fHists[ipad]->GetYaxis();
      double cnt = fHists[ipad]->GetEntries();
      double cntOverThres = fHists[ipad]->Integral(1,nBins,yaxis->FindBin(fChargeThreshold),yaxis->FindBin(rangeYaxis[2]));
      
      qmean[ipad] = fHists[ipad]->GetMean(2);
      hHits->SetBinContent(ipad+1,cnt);
      hHitsOverThres->SetBinContent(ipad+1,cntOverThres);
      if (cnt > 0) hHitsRatio->SetBinContent(ipad+1,cntOverThres/cnt);
      hMean->SetBinContent(ipad+1,qmean[ipad]);
         

      if (cnt < fCountThreshold) {
         isBad[ipad] = true;
      }
      if (cntOverThres/cnt < fCountRatioThreshold) {
         lowCharge[ipad] = true;
         isBad[ipad] = true;
      }
      if (isBad[ipad]) {
         hMaybeBad->SetBinContent(ipad+1,1);
         TH1* h1 = (TH1*)fHists[ipad]->Clone();
         h1->SetDirectory(baddir);
      } else {
      }
   }
   
   // list edges vs event number 
   corrdir->cd();
   std::vector<TH2*> scaled(nPads);

   // diagnosis using probability 
   for (int i =0;  i < nPads; ++i) {
      std::vector<double> scales_after(nBins);
      std::vector<double>& scales = scalesArray[i];
      
      std::vector<double> curvature;
      hHits->SetBinContent(i+1,fHists[i]->GetEntries());
      hMean->SetBinContent(i+1,fHists[i]->GetMean(2));
      
      if (isBad[i]) {
         // low statistics
         fHists[i]->Clone();
         continue;         
      }
      TH2* hsmooth = static_cast<TH2*>(fHists[i]->Clone());
      hsmooth->Smooth();
      

      // diagnosis before correction
      TH1* href = NULL;
      
      href = fHists[i]->ProjectionY("href",2,5);
      scales.resize(nBins,1);
      scaled[i] = (TH2*)fHists[i]->Clone(Form("%s_scaled",fHists[i]->GetName()));
      scaled[i]->Reset();
      int minbin = href->GetYaxis()->FindBin(1000.);
      
      for (int ibin = 0; ibin < nBins; ibin++) {
         TH1* h1 = fHists[i]->ProjectionY("htmp",ibin+1,ibin+1);
         TH1 *h11 = hsmooth->ProjectionY("htmp2",ibin+1,ibin+1);
         double xmax = 0;
         for (int ibin2 = h11->GetNbinsX(); ibin2 > minbin; ibin2--) {
            if (xmax < TMath::Limits<Double_t>::Epsilon() && h11->GetBinContent(ibin2) > 0) {
               xmax = h11->GetXaxis()->GetBinCenter(ibin2);
            } else if (xmax > 0. && h11->GetBinContent(ibin2) < TMath::Limits<Double_t>::Epsilon()) {
               xmax = 0.;
            }
         }
         delete h11;
         xmax *= 1.2;
         
         href->GetXaxis()->SetRangeUser(1000,xmax);
         h1->GetXaxis()->SetRangeUser(1000,xmax);
         scales[ibin] = href->GetMean()/h1->GetMean();
         h1->GetXaxis()->SetRangeUser(1000/scales[ibin],xmax/scales[ibin]);
         scales[ibin] = href->GetMean()/h1->GetMean();
         href->GetXaxis()->UnZoom();
         h1->GetXaxis()->UnZoom();
         
         TH1 *h1s = Util::ScaleX(h1,scales[ibin]);
         for (int iy = 0, nBinsY = scaled[i]->GetNbinsY(); iy < nBinsY; ++iy ) {
            scaled[i]->SetBinContent(ibin+1,iy+1,h1s->GetBinContent(iy+1));
         }
         delete h1;
         delete h1s;
      }
      delete href;
      delete hsmooth;
      
      const int cnt = 5;


      double ave = std::accumulate(scales.begin(),scales.end(),0.) / scales.size();
      double var = std::inner_product(scales.begin(),scales.end(),scales.begin(),0.) / scales.size() - ave * ave;
      double ave_after = std::accumulate(scales_after.begin(),scales_after.end(),0.) / scales_after.size();
      
      hDiag->Fill(i,TMath::Sqrt(var)/ave_after);
      
      if (scales.size() > cnt) {
         // diagnosis of existence of gain shift
         double smin = TMath::Limits<Double_t>::Max();
         double smax = TMath::Limits<Double_t>::Min();
         double sum =  0.;
         double sum2 =  0.;
         std::vector<double> scaleave(scales.size()-cnt + 1);
         std::vector<double> scaledev(scales.size()-cnt + 1);

         for (int j = 0; j < cnt; ++j) {
            sum += scales[j];
            sum2 += scales[j] * scales[j];
         }
         scaleave[0] = sum/cnt;
         scaledev[0] = sum2/cnt - (scaleave[0] * scaleave[0]);
         
         for (int j = 0, n = scales.size() - cnt; j < n ; ++j) {
            sum = sum - scales[j] + scales[j+cnt];
            sum2 = sum2 - scales[j] * scales[j] + scales[j+cnt] * scales[j+cnt];
            scaleave[j+1] = sum/cnt;
            scaledev[j+1] = sum2/cnt - scaleave[j+1] * scaleave[j+1];
         }

         for (int j = 0, n = scaleave.size(); j < n; ++j) {
            hEval->Fill(i,scaleave[j]);
            for (int k = 0; k < n; ++k) {
               diaghists[i]->SetBinContent(j+1,k+1,TMath::Abs((scaleave[j]-scaleave[k]))/TMath::Sqrt(scaledev[j]+scaledev[k]));
               if (j==k) {
                  diaghists[i]->SetBinContent(j+1,k+1,TMath::Sqrt(scaledev[j]));
                  
               }
            }
         }

         hMaxDeviation->SetBinContent(i+1,diaghists[i]->GetMaximum());
         if (diaghists[i]->GetMaximum() > fDeviationThreshold) {
            hasGainShift[i] = true;
            hMayHaveShift->SetBinContent(i+1,1);
            TH1* h1 = (TH1*)fHists[i]->Clone();
            h1->SetDirectory(shiftdir);
            TH1* h2 = (TH1*)scaled[i]->Clone();
            h2->SetDirectory(corrdirShift);
         }
      }
   }

   file->Write();


   // output to file
   std::ofstream fout(fCorrectionFilename.Value());
   if (!fout) {
      Error(__func__,"Cannot open file '%s'",fCorrectionFilename.Value().Data());
      return;
   }

   const char* delim = " ";
   for (int ipad = 0; ipad < nPads; ++ipad) {
      for (int ibin = 0; ibin < nBins; ++ibin) {
         fout << ipad << delim
              << eventBorders[ibin] << delim
              << 0. << delim
              << scalesArray[ipad][ibin] << std::endl;
      }
   }

   fout.close();


   Info(__func__,"Diagnosis done for %d entries selected from %d events in %s%04d",nEntries,maxEventNumber,runName.Data(),runNumber);
   Info(__func__,"  Total  %d pads",nPads);
   Info(__func__,"  Bad    %zu pads CntThres = %f, RatioThres = %f",std::count(isBad.begin(),isBad.end(),true),fCountThreshold.Value(),fCountRatioThreshold.Value());
   Info(__func__,"  Shift  %zu pads Thres = %f",std::count(hasGainShift.begin(),hasGainShift.end(),true),fDeviationThreshold.Value());
   Info(__func__,"  Low Charge %zu pads ChargeThres = %f, RatioThres = %f",std::count(lowCharge.begin(),lowCharge.end(),true),fChargeThreshold.Value(),fCountRatioThreshold.Value());
   
   

   

   

//   file->Close();
   Info(__func__,"Done");
}

