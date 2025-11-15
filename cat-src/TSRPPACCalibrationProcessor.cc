/**
 * @file   TSRPPACCalibrationProcessor.cc
 * @brief
 *
 * @date   Created:       2019-08-30 17:57:30 JST (ota)
 *         Last Modified: 2019-08-30 19:08:19 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 OTA Shinsuke All rights reserved.
 */

#include "TSRPPACCalibrationProcessor.h"
#include "TSRPPACPlaneData.h"
#include "constant.h"
#include "TGateArray.h"
#include "TGateObject.h"

#include <TROOT.h>
#include <TClonesArray.h>
#include <TH1.h>
#include <TClass.h>

#include <TFolder.h>

#include "TSRPPACPlaneCalibrationTask.h"

using art::TSRPPACCalibrationProcessor;

ClassImp(art::TSRPPACCalibrationProcessor)

namespace {
   const TString DEFAULT_GATE = "default_gate";
   const TString DIR_NAME     = "SRPPACCalibHists";
}

const char* const TSRPPACCalibrationProcessor::kTASKNAME = "SRPPACCalibration";

// Default constructor
TSRPPACCalibrationProcessor::TSRPPACCalibrationProcessor()
   : fHasTimingGate(kFALSE), fHasChargeGate(kFALSE), fGate(NULL),
     fOwnsTask(kFALSE), fTask(NULL)
{
   FloatVec_t defTimingGate;
   defTimingGate.push_back(0);
   defTimingGate.push_back(10000);
   const FloatVec_t defChargeGate(2,art::kInvalidD);

   RegisterInputCollection("InputCollection","mwdc hit array",
			   fInputColName,TString("srppac_x"),
			   &fPlaneIn, "TClonesArray", "art::TSRPPACPlaneData");
   RegisterProcessorParameter("TimingGate","timing gate (float)",
			      fTimingGate,defTimingGate);
   RegisterProcessorParameter("ChargeGate","charge gate (float)",
			      fChargeGate,defChargeGate);
   RegisterOptionalParameter("Gate","name of gate object",
			     fGateName,DEFAULT_GATE);
   RegisterProcessorParameter("Combination","combination of charge: 0 => ID0, ID1, 1 => ID1, ID2",
                              fCombination,0);
   RegisterOptionalParameter("UseMax","use max charge hit for the calibration",
			     fUseMax,kFALSE);
}

TSRPPACCalibrationProcessor::~TSRPPACCalibrationProcessor()
{
   if (fOwnsTask) delete fTask;
}

void TSRPPACCalibrationProcessor::Init(TEventCollection *col)
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
      SetStateError("charge gate: off");
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

void TSRPPACCalibrationProcessor::BeginOfRun()
{
   {
      const TString &hName = TString::Format("%sc%d",fInputColName.Data(),fCombination);
      const TString &hTitle = TString::Format("%s;#it{t}_{Leading};count",fInputColName.Data());
      const Int_t binsPerNanoSec = 5;
      const Int_t nBins = (Int_t) ((fChargeGate[1] - fChargeGate[0]) * binsPerNanoSec );
      TDirectory *current_dir = TDirectory::CurrentDirectory();
      TDirectory *dir = dynamic_cast<TDirectory*>(gROOT->FindObject(DIR_NAME));
      if (!dir) dir = new TDirectory(DIR_NAME,"SRPPAC calibration","",gROOT);
      dir->cd();
      fH = new TH1D(hName,hTitle,nBins,fChargeGate[0],fChargeGate[1]);
      current_dir->cd();
   }

   const TString kLOOP_FOLDER_PATH("artemis/loops/loop0");

   TFolder *f = static_cast<TFolder*>(gROOT->FindObjectAny(kLOOP_FOLDER_PATH));
   fTask = static_cast<TSRPPACPlaneCalibrationTask*>(f->FindObjectAny(kTASKNAME));

   if (!fTask) {
      fTask =
	 new TSRPPACPlaneCalibrationTask(kTASKNAME,"SRPPAC plane calibration task");;
      f->Add(fTask);
      gROOT->GetListOfBrowsables()->Add(fTask);
      fOwnsTask = kTRUE;
   }

   fTask->AddHist(fH);
}

void TSRPPACCalibrationProcessor::Process()
{
   if (!fHasTimingGate) return;

   if (fCombination == 0 && (*fPlaneIn)->GetEntriesFast() > 1) {
      const TSRPPACPlaneData *const data0 = static_cast<TSRPPACPlaneData*>((*fPlaneIn)->UncheckedAt(0));
      const TSRPPACPlaneData *const data1 = static_cast<TSRPPACPlaneData*>((*fPlaneIn)->UncheckedAt(1));
      fH->Fill(data0->GetCharge() - data1->GetCharge());
   } else if (fCombination == 1 && (*fPlaneIn)->GetEntriesFast() > 2) {
      const TSRPPACPlaneData *const data0 = static_cast<TSRPPACPlaneData*>((*fPlaneIn)->UncheckedAt(1));
      const TSRPPACPlaneData *const data1 = static_cast<TSRPPACPlaneData*>((*fPlaneIn)->UncheckedAt(2));
      fH->Fill(data0->GetCharge() - data1->GetCharge());
   }
      

#if 0   
   for (Int_t iHit = 0, nHit = (*fPlaneIn)->GetEntriesFast(); iHit != nHit; ++iHit) {

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
	 fH->Fill( charge );
	 if (fUseMax) return;
      }
   }
#endif
}
