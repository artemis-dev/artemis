/**
 * @file   TMWDCPlaneCalibrationTask.cc
 * @brief  task object for MWDC Plane calibration
 *
 * @date   Created       : 2014-04-02 10:36:00 JST
 *         Last Modified : 2014-04-04 12:43:08 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TMWDCPlaneCalibrationTask.h"

#include "TGMWDCCalibration.h"
#include <TGClient.h>
#include <TStyle.h>
#include <TBrowser.h>
#include <TObjArray.h>
#include <TH1D.h>

using art::TMWDCPlaneCalibrationTask;

ClassImp(TMWDCPlaneCalibrationTask)

TMWDCPlaneCalibrationTask::TMWDCPlaneCalibrationTask()
   : fMC(NULL) {
}

TMWDCPlaneCalibrationTask::TMWDCPlaneCalibrationTask(const char* name, const char* title)
   : TTask(name, title), fMC(NULL), fHists(NULL)
{
}

TMWDCPlaneCalibrationTask::~TMWDCPlaneCalibrationTask()
{
   delete fMC;
   delete fHists;
}

TMWDCPlaneCalibrationTask::TMWDCPlaneCalibrationTask(const TMWDCPlaneCalibrationTask& rhs)
   : TTask(rhs)
{
   if (rhs.fHists) {
      fHists = (TObjArray*)rhs.fHists->Clone();
   }
}

TMWDCPlaneCalibrationTask& TMWDCPlaneCalibrationTask::operator=(const TMWDCPlaneCalibrationTask& rhs)
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

void TMWDCPlaneCalibrationTask::Exec(Option_t*)
{
   if (fMC) delete fMC;
   fMC = new TGMWDCCalibration(gClient->GetDefaultRoot(),
			       gStyle->GetCanvasDefW(),
			       gStyle->GetCanvasDefH(),
			       fHists);
}

void TMWDCPlaneCalibrationTask::Browse(TBrowser *b)
{
   b->Add(fHists);
   TTask::Browse(b);
}


void TMWDCPlaneCalibrationTask::AddHist(TH1D *h)
{
   if (!fHists) {
      fHists = new TObjArray();
      fHists->SetName("hists");
      fHists->SetOwner(kTRUE);
   }
   fHists->Add(h);
}
