/**
 * @file   TCmdMWDCCalib.cc
 * @brief
 *
 * @date   Created:       2014-01-17 13:14:45
 *         Last Modified: 2014-07-02 13:16:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TCmdMWDCCalib.h"

#include <TROOT.h>
#include <TTask.h>

using art::TCmdMWDCCalib;

ClassImp(art::TCmdMWDCCalib)

// Default constructor
TCmdMWDCCalib::TCmdMWDCCalib() {
   SetName("mwdccalib");
   SetTitle("launch MWDC calibrator");
}

TCmdMWDCCalib::~TCmdMWDCCalib() {
}

TCmdMWDCCalib* TCmdMWDCCalib::Instance() {
   static TCmdMWDCCalib instance;
   return &instance;
}

Long_t TCmdMWDCCalib::Cmd(vector<TString>) {
   return Run();
}

Long_t TCmdMWDCCalib::Run() {
   TTask *task = dynamic_cast<TTask*>(gROOT->FindObjectAny("MWDCCalibration"));
   if (task) task->ExecuteTask();
   return 1;
}
