/**
 * @file   TMWDCCalibrationProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-18 17:08:53
 *         Last Modified: 2014-06-29 05:34:26 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCCalibrationProcessor.h"
#include "TMWDCHitData.h"
#include "constant.h"
#include "TGateArray.h"
#include "TGateObject.h"

#include <TROOT.h>
#include <TClonesArray.h>
#include <TH1.h>
#include <TClass.h>

#include <TFolder.h>

#include "TMWDCPlaneCalibrationTask.h"

using art::TMWDCCalibrationProcessor;

ClassImp(art::TMWDCCalibrationProcessor)

namespace {
   const TString DEFAULT_GATE = "default_gate";
   const TString DIR_NAME     = "MWDCCalibHists";
}

const char* const TMWDCCalibrationProcessor::kTASKNAME = "MWDCCalibration";

// Default constructor
TMWDCCalibrationProcessor::TMWDCCalibrationProcessor()
   : fHasTimingGate(kFALSE), fHasChargeGate(kFALSE), fGate(NULL),
     fOwnsTask(kFALSE), fTask(NULL)
{
   FloatVec_t defTimingGate;
   defTimingGate.push_back(0);
   defTimingGate.push_back(10000);
   const FloatVec_t defChargeGate(2,art::kInvalidD);

   RegisterInputCollection("InputCollection","mwdc hit array",
			   fInputColName,TString("mwdc1_x"),
			   &fPlaneIn, "TClonesArray", "art::TMWDCHitData");
   RegisterProcessorParameter("TimingGate","timing gate (float)",
			      fTimingGate,defTimingGate);
   RegisterProcessorParameter("ChargeGate","charge gate (float)",
			      fChargeGate,defChargeGate);
   RegisterOptionalParameter("Gate","name of gate object",
			     fGateName,DEFAULT_GATE);
   RegisterOptionalParameter("UseMax","use max charge hit for the calibration",
			     fUseMax,kFALSE);
}

TMWDCCalibrationProcessor::~TMWDCCalibrationProcessor()
{
   if (fOwnsTask) delete fTask;
}

void TMWDCCalibrationProcessor::Init(TEventCollection *col)
{
   if ( IsValid(fTimingGate[0]) && IsValid(fTimingGate[1] ) ) {
      Info("Init","timing gate: (%f:%f)",fTimingGate[0],fTimingGate[1]);
      fHasTimingGate = kTRUE;
   } else {
      Info("Init","timing gate: off");
   }
   if ( IsValid(fChargeGate[0]) && IsValid(fChargeGate[1] ) ) {
      Info("Init","charge gate: (%f:%f)",fChargeGate[0],fChargeGate[1]);
      fHasChargeGate = kTRUE;
   } else {
      Info("Init","charge gate: off");
   }

   if (!fGateName.EqualTo(DEFAULT_GATE)) {
      TGateArray **const gateArray = reinterpret_cast<TGateArray**>(col->GetObjectRef("gate"));
      if(!gateArray) {
	 SetStateError("gate array not found. Run TGateArrayInitializer before this.");
	 return;
      }

      fGate = (*gateArray)->GetObjectRef(fGateName);
   }
}

void TMWDCCalibrationProcessor::BeginOfRun()
{
   {
      const TString &hName = TString::Format("%s",fInputColName.Data());
      const TString &hTitle = TString::Format("%s;#it{t}_{Leading};count",fInputColName.Data());
      const Int_t binsPerNanoSec = 5;
      const Int_t nBins = (Int_t) ((fTimingGate[1] - fTimingGate[0]) * binsPerNanoSec );
      TDirectory *current_dir = TDirectory::CurrentDirectory();
      TDirectory *dir = dynamic_cast<TDirectory*>(gROOT->FindObject(DIR_NAME));
      if (!dir) dir = new TDirectory(DIR_NAME,"MWDC calibration","",gROOT);
      dir->cd();
      fH = new TH1D(hName,hTitle,nBins,fTimingGate[0],fTimingGate[1]);
      current_dir->cd();
   }

   const TString kLOOP_FOLDER_PATH("artemis/loops/loop0");

   TFolder *f = static_cast<TFolder*>(gROOT->FindObjectAny(kLOOP_FOLDER_PATH));
   fTask = static_cast<TMWDCPlaneCalibrationTask*>(f->FindObjectAny(kTASKNAME));

   if (!fTask) {
      fTask =
	 new TMWDCPlaneCalibrationTask(kTASKNAME,"MWDC plane calibration task");;
      f->Add(fTask);
      gROOT->GetListOfBrowsables()->Add(fTask);
      fOwnsTask = kTRUE;
   }

   fTask->AddHist(fH);
}

void TMWDCCalibrationProcessor::Process()
{
   if (!fHasTimingGate) return;

   for (Int_t iHit = 0, nHit = (*fPlaneIn)->GetEntriesFast(); iHit != nHit; ++iHit) {
      const TMWDCHitData *const data = static_cast<TMWDCHitData*>((*fPlaneIn)->At(iHit));

      const Double_t timing = data->GetTiming();
      if ( !IsValid(timing)
	   || fTimingGate[0] > timing
	   || fTimingGate[1] < timing ) continue;

      const Double_t charge = data->GetCharge();
      if (fHasChargeGate) {
	 if ( !IsValid(charge)
	      || fChargeGate[0] > charge
	      || fChargeGate[1] < charge ) continue;
      }

      if (!fGate || (*fGate)->IsTrue()) {
	 fH->Fill( timing );
	 if (fUseMax) return;
      }
   }
}
