/* @file TCatEvalTpcChargeResolution.cc
 * @brief evaluation of charge resolution of tpc
 *
 * @date Create        : 2020-07-22 06:47:10 JST
 *       Last Modified : 2020-07-28 12:56:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCatEvalTpcChargeResolution.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCatTpcTrack.h"
#include "TCatReadoutPadArray.h"
#include "TROOT.h"
#include "TFile.h"
#include "TArtemisUtil.h"
#include "TCatCmdAvy.h"
#include "TCatCmdHstore.h"
#include "TAnalysisInfo.h"

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


ClassImp(art::TCatEvalTpcChargeResolution)

using art::TCatEvalTpcChargeResolution;

TCatEvalTpcChargeResolution::TCatEvalTpcChargeResolution()
{
   DoubleVec_t xaxis(3); 
   DoubleVec_t yaxis(3);
   xaxis[0] = 100;
   xaxis[1] = 0.;
   xaxis[2] = 0.1;
   yaxis[0] = 100;
   yaxis[1] = 0.;
   yaxis[2] = 2;
   
   Register(fInput("Input","Input track","resTrack"));
   Register(fDoUseHits("DoUseHits","flag to use each hit (track.fHits) instead of representative (track): default false",false));
   Register(fReadoutPadArray("ReadoutPadArray","name of readout pad array","readoutPad"));
   Register(fXaxis("Xaxis","[nbinsx, xmin, xmax]",xaxis));
   Register(fYaxis("Yaxis","[nbinsy, ymin, ymax]",yaxis));
   Register(fOutputFilename("OutputFile","output file name","temp.root"));
   
}

TCatEvalTpcChargeResolution::~TCatEvalTpcChargeResolution()
{
}

void TCatEvalTpcChargeResolution::Init(TEventCollection* col)
{
   const int nPads = fReadoutPadArray->GetEntries();
   const DoubleVec_t& xaxis = static_cast<DoubleVec_t>(fXaxis);
   const DoubleVec_t& yaxis = static_cast<DoubleVec_t>(fYaxis);
   fHists.resize(nPads);
   
   // count digits
   int digit = 1;
   {
      int x = nPads;
      while ((x = (x/10))) ++digit;
   }
   TDirectory *saved = gDirectory;
   gROOT->mkdir(GetName())->cd();
   TString idfmt = Form("%%0%dd",digit);
   for (int i = 0; i < nPads; ++i) {
      TString idstr = Form(idfmt.Data(),i);
      fHists[i] = new TH2F(Form("h%s",idstr.Data()),Form("ratio vs edep (%s)",idstr.Data()),
                           xaxis[0],xaxis[1],xaxis[2],yaxis[0],yaxis[1],yaxis[2]);
   }
   saved->cd();
   
}

void TCatEvalTpcChargeResolution::Process()
{
   if (!fInput->GetEntriesFast()) return;
   const int nPads = fReadoutPadArray->GetEntries();
   for (int i = 0, n = fInput->GetEntriesFast(); i < n; ++i)    {
      TCatTpcTrack* track = static_cast<TCatTpcTrack*>(fInput->UncheckedAt(i));
      if (fDoUseHits) {
         for (int ihit = 0, nhits = track->GetHits().size(); ihit < nhits; ++ihit) {
            if (track->GetHits()[ihit]->GetID() > nPads) continue;
            const TCatTrackResultPulse* pulse = static_cast<TCatTrackResultPulse*>(track->GetHits()[ihit]);
            const int id = pulse->GetID();
            if (id >= nPads) continue;
            const double charge = pulse->GetCharge();
            const double edep = pulse->GetEnergyDeposit();
            fHists[id]->Fill(edep,charge/edep);
         }
      } else {
         const int id = track->GetID();
         if (id >= nPads) continue;
         const double charge = track->GetCharge();
         const double edep = track->GetEnergyDeposit();
         fHists[id]->Fill(edep,charge/edep);
         
      }
   }
}

void TCatEvalTpcChargeResolution::EndOfRun()
{
   TString filename = fOutputFilename;
   if (filename.IsNull()) return;
#ifdef USE_MPI
   int myrank, npe,useMPI;
   MPI_Initialized(&useMPI);
   if (useMPI) {
      MPI_Comm_size(MPI_COMM_WORLD, &npe);
      MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
      filename = Form("%s%d",filename.Data(),myrank);
   }
   Info("EndOfRun","%s created UseMPI = %d, myrank = %d, npe = %d\n",filename.Data(),useMPI,myrank,npe);
#endif

   TFile *file = TFile::Open(filename,"recreate");
   
   TDirectory * maindir = file->mkdir(GetName());
   TDirectory * paddir = maindir->mkdir("pads");
   const int nPads = fReadoutPadArray->GetEntriesFast();
   for (int i = 0, n = fHists.size(); i < n; ++i) {
      if (fHists[i]->GetEntries() < 10) continue;
      TString dirname = Form("d%s",fHists[i]->GetName());
      paddir->mkdir(dirname)->cd();
      fHists[i]->SetDirectory(gDirectory);
   }
   TAnalysisInfo::AddTo(file);
   file->Write();
   file->Close();
   
#ifdef USE_MPI
   if (useMPI) {
      MPI_Barrier(MPI_COMM_WORLD);   
   }
   Util::MPIFileMerger(TString(fOutputFilename).Data());

   if (npe > 0 && myrank != 0) {
      Info("EndOfRun","return by myrank %d",myrank);
      if (useMPI) {
         MPI_Barrier(MPI_COMM_WORLD);   
      }
      return;      
   } else {
      Info("EndOfRUn","Continue for myrank %d", myrank);
      
   }
#endif
   file = TFile::Open(TString(fOutputFilename).Data(),"update");
   if (!file) {
      Error("No such file %s",TString(fOutputFilename).Data());
      return;
   }
   Info("EndOfRun","output file is ready");
   maindir = dynamic_cast<TDirectory*>(file->Get(GetName()));
   if (!maindir) {
      Error("EndOfRun","No such directory %s",GetName());
      return;      
   }
   Info("EndOfRun","Directory %s exists",GetName());
   paddir = dynamic_cast<TDirectory*>(maindir->Get("pads"));
   if (!paddir) {
      Error("EndOfRun","No such directory pads in %s",maindir->GetName());
      return;
      
   }
   maindir->cd();
   std::vector<double> s0(nPads), s1(nPads), s2(nPads);
   TCatCmdAvy *avy = TCatCmdAvy::Instance();
   // histograms for macroscopic behavior
   std::vector<TH1*> hSigPar2(3);
   std::vector<TH1*> hSigPar(3);
   hSigPar2[0] = new TH1F("hSigPar0id","SigPar0 vs ID",nPads,-0.5,nPads-0.5);
   hSigPar2[1] = new TH1F("hSigPar1id","SigPar1 vs ID",nPads,-0.5,nPads-0.5);
   hSigPar2[2] = new TH1F("hSigPar2id","SigPar2 vs ID",nPads,-0.5,nPads-0.5);
   hSigPar[0] = new TH1F("hSigPar0","SigPar0",1000,-1.,1.);
   hSigPar[1] = new TH1F("hSigPar1","SigPar1",1000,0.,5);
   hSigPar[2] = new TH1F("hSigPar2","SigPar2",1000,-1.,1.);
   
   TH2* hist = NULL;
   Info("EndOfRun","Loop for %d pads",nPads);
   int digit = 1;
   {
      int x = nPads;
      while ((x = (x/10))) ++digit;
   }
   TString idfmt = Form("%%0%dd",digit);
   TString dirfmt = Form("dh%s",idfmt.Data());
   TString histfmt = Form("h%s",idfmt.Data());
   for (int i = 0, n = nPads; i < n; ++i) {
      TString dirname = Form(dirfmt.Data(),i);
      TString histname = Form(histfmt.Data(),i);
      if (!paddir->FindKey(dirname)) {
         Error("EndOfRun","No such directory %s",dirname.Data());
         continue;
      } else {
         Info("EndOfRun","process %s",dirname.Data());
      }
      
      paddir->cd(dirname);
      hist = static_cast<TH2*>(gDirectory->Get(histname));
      if (!hist) {
         Error("EndOfRun","No such hist %s",histname.Data());
         continue;
      } else {
         Info("EndOfRun","Process %s",histname.Data());
      }
      Info("EndOfRun","avp = %p",avy);
      
      avy->Run(hist);
      Info("EndOfRun","Getting new histogram");
      TString name = Form("%ssigma_y",hist->GetName());
      TH1* hsigma = static_cast<TH1*>(gDirectory->Get(name));
      if (!hsigma) {
         Error("EndOfRun","No such hist %s",name.Data());
         continue;
      } else {
         Info("EndOfRun","Process %s",name.Data());
         
      }
      TF1 *fun = new TF1("fun","TMath::Sqrt([0]*[0] + [1]*[1]*x/37e-6 + [2]*[2]*x*x/37e-6/37e-6)/(x/37e-6)",0.,1.);
      hsigma->Fit(fun,"","",0,0.05);
      for (int ipar = 0; ipar < 3; ++ipar) {
         hSigPar2[ipar]->SetBinContent(i+1,fun->GetParameter(ipar));
         hSigPar2[ipar]->SetBinError(i+1,fun->GetParError(ipar));
         hSigPar[ipar]->Fill(fun->GetParameter(ipar));
      }
      hist->Delete();
   }
   file->Write();
   file->Close();
   Info("EndOfRun","done");
   
}
