/**
 * @file   TMWDCDriftLengthCorrectionProcessor.cc
 * @brief  make drift length correction coefficient file
 *
 * @date   Created       : 2018-07-13 17:58:13 JST
 *         Last Modified :
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#include "TMWDCDriftLengthCorrectionProcessor.h"

#include "TDirectory.h"
#include "TTask.h"
#include "TMWDCPlaneCalibrationTask.h"
#include "TH1D.h"
#include "TFolder.h"
#include "TMWDCTrackingResult.h"
#include "TMWDCHitData.h"
#include "TClonesArray.h"
#include "TROOT.h"

// #include "TEventDisplayProcessorHelper.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH2D.h"

using art::TMWDCDriftLengthCorrectionProcessor;

ClassImp(art::TMWDCDriftLengthCorrectionProcessor)
namespace {
   const TString DEFAULT_GATE = "default_gate";
   const TString DIR_NAME     = "MWDCDlcorHists";
}

const char* const TMWDCDriftLengthCorrectionProcessor::kTASKNAME = "MWDCDriftLengthCorrection";

TMWDCDriftLengthCorrectionProcessor::TMWDCDriftLengthCorrectionProcessor()
   : fOwnsTask(kFALSE), fTask(NULL)
{
   Double_t defCellSize = 9.;
   
   RegisterInputCollection("InputTrack", "mwdc track",
                           fInputTrack, TString("dcx1"),
                           &fTrackIn, "TClonesArray", "art::TMWDCTrackingResult");
   RegisterInputCollection("InputPlane1", "mwdc plane1 (correction target)",
                           fInputPlane1, TString("dcx1_x1"),
                           &fPlaneIn1, "TClonesArray", "art::TMWDCHitData");
   RegisterInputCollection("InputPlane2", "mwdc plane2 (reference)",
                           fInputPlane2, TString("dcx1_x2"),
                           &fPlaneIn2, "TClonesArray", "art::TMWDCHitData");
   RegisterProcessorParameter("CellSize", "mwdc cell size",
                              fCellSize, defCellSize);
   RegisterProcessorParameter("Verbose", "verbose level (1: non quiet)",
                              fVerbose, 0);

   // fEventDisplayHelper = new TEventDisplayProcessorHelper;
   // if (fVerbose > 1)
//      printf("TEventDisplayHelper object is created\n");
   // RegisterHelper(fEventDisplayHelper);
}

TMWDCDriftLengthCorrectionProcessor::~TMWDCDriftLengthCorrectionProcessor()
{
   if (fOwnsTask) delete fTask;
//   if (fVerbose > 1) delete fEventDisplayHelper;
}

void TMWDCDriftLengthCorrectionProcessor::Init(TEventCollection *col)
{
//    if (fVerbose > 1) {
//       if (!fEventDisplayHelper->Displays()) {
//          printf("fEventDisplayHelper->Displays() Failed\n");
//          return;
//       }

// //      fBlankHist = new TH2D("canvas","canvas",600,-300,300,500,0,10);
// //      fBlankHist->SetStats(0);
      
//       // fBlankHist = new TH2D("canvas","canvas",600,-300,300,500,0,10);
//       // fBlankHist->SetStats(0);

//       fEventDisplayHelper->Process();
//    }

}
void TMWDCDriftLengthCorrectionProcessor::BeginOfRun()
{
   {
      const TString &hName  = TString::Format("%s",fInputPlane1.Data());
      const TString &hTitle = TString::Format("%s;#it{dl}_{eff};count",fInputPlane1.Data());
      const TString &hName1 = TString::Format("%sadopt",fInputPlane1.Data());
      const TString &hTitle1 = TString::Format("%s;#it{dl}_{adopt};count",fInputPlane1.Data());
      const TString &hName2 = TString::Format("%sinv",fInputPlane2.Data());
      const TString &hTitle2 = TString::Format("%s;#it{dl}_{inv}",fInputPlane2.Data());
      const Int_t binsPermm = 100;
      const Int_t nBins = (Int_t) (fCellSize/2. * binsPermm);
      TDirectory *current_dir = TDirectory::CurrentDirectory();
      TDirectory *dir = dynamic_cast<TDirectory*>(gROOT->FindObject(DIR_NAME));
      if (!dir) dir = new TDirectory(DIR_NAME, "MWDC DL Correction","",gROOT);
      dir->cd();

      fH  = new TH1D(hName, hTitle, nBins,0,fCellSize/2.);
      fH1 = new TH1D(hName1,hTitle1,nBins,0,fCellSize/2.);
      fH2 = new TH1D(hName2,hTitle2,nBins,0,fCellSize/2.);
      
   }
   const TString kLOOP_FOLDER_PATH("artemis/loops/loop0");

   TFolder *f = static_cast<TFolder*>(gROOT->FindObjectAny(kLOOP_FOLDER_PATH));
   fTask = static_cast<TMWDCPlaneCalibrationTask*>(f->FindObjectAny(kTASKNAME));

   if (!fTask) {
      fTask = new TMWDCPlaneCalibrationTask(kTASKNAME,"MWDC drift length correction task");
      f->Add(fTask);
      gROOT->GetListOfBrowsables()->Add(fTask);
      fOwnsTask = kTRUE;
   }

   fTask->AddHist(fH);
}

void TMWDCDriftLengthCorrectionProcessor::Process()
{
//    if (fVerbose > 1) {
//       if (!fEventDisplayHelper->Displays()) return;
// //      if (fPad) fPad->Clear();
//    }
//   for (Int_t iHi(*fTrackIn)->GetEntriesFast(); iHit != n
   const TMWDCTrackingResult *const track = static_cast<TMWDCTrackingResult*>((*fTrackIn)->At(0));
//   const TMWDCHitData *const plane1data = static_cast<TMWDCHitData*>((*fInputPlane1)->At(0));

   if ((*fPlaneIn1)->GetEntriesFast()==0 && (*fPlaneIn2)->GetEntriesFast()!=0) {

      const TMWDCHitData *const plane2data = static_cast<TMWDCHitData*>((*fPlaneIn2)->At(0));
      const Double_t plane2dl = plane2data->GetDriftLength();
      if (fVerbose) {
         
         printf("plane 2 dl = %f\n",plane2dl);
      }
      fH2->Fill(fCellSize/2. - plane2data->GetDriftLength());
      fH->Fill(fCellSize/2. - plane2data->GetDriftLength());
//      fH2->Fill(fCellSize/2.);
   }

   Int_t planeID = -1;   
   for (Int_t iPlane = 0; iPlane != track->GetNPlane(); ++iPlane) {
//      if (fVerbose) printf("fInputPlane1Name = %s, TrackPlaneName = %s, planeID = %d\n",fInputPlane1.Data(), (track->GetPlaneName(iPlane)).Data(),iPlane);

      if(fInputPlane1.EqualTo(track->GetPlaneName(iPlane))){
//      if (fVerbose) printf("Matched plane\n");
         planeID = iPlane;
         break;
      }
   }
   if (planeID < 0) {
//      if (fVerbose) printf("Invalid plane\n");
      return;
   }
   
   const Double_t plane1DriftLengthAdopt = track->GetDriftLengthAdopted(planeID);
//   if (fVerbose) printf("%s x = %f, DL Adopted = %f\n",fInputPlane1.Data(), track->GetX(),plane1DriftLengthAdopt);
   fH1->Fill(plane1DriftLengthAdopt);
   fH->Fill(plane1DriftLengthAdopt);

   if (fVerbose) {
      printf("plane1 multiplicity = %d, plane2 multiplicity != %d\n",
                (*fPlaneIn1)->GetEntriesFast(),(*fPlaneIn2)->GetEntriesFast());
   }
   
   // fH = static_cast<TH1D*>(fH1->Clone());
   // fH->Add(fH2);

//    if (fVerbose > 1) {
//       fPad = fEventDisplayHelper->GetMainPad();
// //      pad->cd(0);
// //      fBlankHist->Draw();
//       fPad->Divide(2,2);
//       fPad->cd(1);
//       fH1->Draw();
//       fPad->cd(2);
//       fH2->Draw();
//       fPad->cd(3);
//       fH->Draw();

//       fEventDisplayHelper->Process();
//       fPad->Clear();      
//    }

}


// void TMWDCDriftLengthCorrectionProcessor::PostLoop()
// {
//    fH->Reset();
//    fH->Add(fH1);
//    fH->Add(fH2);
// }
