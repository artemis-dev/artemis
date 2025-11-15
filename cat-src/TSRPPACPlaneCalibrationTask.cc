/**
 * @file   TSRPPACPlaneCalibrationTask.cc
 * @brief  task object for SRPPAC Plane calibration based on TMWDCPlaneCalibrationTask
 *
 * @date   Created      : 2019-08-30 18:31:01 JST (ota)
 *         Last Modified: 2019-08-30 18:31:21 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 OTA Shinsuke
 */

#include "TSRPPACPlaneCalibrationTask.h"

#include "TGSRPPACCalibration.h"
#include <TGClient.h>
#include <TStyle.h>
#include <TBrowser.h>
#include <TObjArray.h>
#include <TH1D.h>

using art::TSRPPACPlaneCalibrationTask;

ClassImp(TSRPPACPlaneCalibrationTask)

TSRPPACPlaneCalibrationTask::TSRPPACPlaneCalibrationTask()
   : fMC(NULL) {
}

TSRPPACPlaneCalibrationTask::TSRPPACPlaneCalibrationTask(const char* name, const char* title)
   : TTask(name, title), fMC(NULL), fHists(NULL)
{
}

TSRPPACPlaneCalibrationTask::~TSRPPACPlaneCalibrationTask()
{
   delete fMC;
   delete fHists;
}

TSRPPACPlaneCalibrationTask::TSRPPACPlaneCalibrationTask(const TSRPPACPlaneCalibrationTask& rhs)
   : TTask(rhs)
{
   if (rhs.fHists) {
      fHists = (TObjArray*)rhs.fHists->Clone();
   }
}

TSRPPACPlaneCalibrationTask& TSRPPACPlaneCalibrationTask::operator=(const TSRPPACPlaneCalibrationTask& rhs)
{
   if (this != &rhs) {
      TTask::operator=(rhs);
      delete fMC;
      fMC = NULL;
      delete fHists;
      if (rhs.fHists) {
	 fHists = (TObjArray*)rhs.fHists->Clone();
      }
   }
   return *this;
}

void TSRPPACPlaneCalibrationTask::Exec(Option_t*)
{
   if (fMC) delete fMC;
   fMC = new TGSRPPACCalibration(gClient->GetDefaultRoot(),
			       gStyle->GetCanvasDefW(),
			       gStyle->GetCanvasDefH(),
			       fHists);
}

void TSRPPACPlaneCalibrationTask::Browse(TBrowser *b)
{
   b->Add(fHists);
   TTask::Browse(b);
}


void TSRPPACPlaneCalibrationTask::AddHist(TH1D *h)
{
   if (!fHists) {
      fHists = new TObjArray();
      fHists->SetName("hists");
      fHists->SetOwner(kTRUE);
   }
   fHists->Add(h);
}
