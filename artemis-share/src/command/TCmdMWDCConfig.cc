/**
 * @file   TCmdMWDCConfig.cc
 * @brief
 *
 * @date   Created:       2014-01-17 13:14:45
 *         Last Modified: 2014-07-10 17:32:11 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TCmdMWDCConfig.h"
#include "TGMWDCConfig.h"

#include <TROOT.h>
#include <TTask.h>

using art::TCmdMWDCConfig;

ClassImp(art::TCmdMWDCConfig)

// Default constructor
TCmdMWDCConfig::TCmdMWDCConfig()
{
   SetName("mwdcconfig");
   SetTitle("launch MWDC configurator");
}

TCmdMWDCConfig::~TCmdMWDCConfig() {}

TCmdMWDCConfig* TCmdMWDCConfig::Instance()
{
   static TCmdMWDCConfig instance;
   return &instance;
}

Long_t TCmdMWDCConfig::Cmd(vector<TString>)
{
   return Run();
}

Long_t TCmdMWDCConfig::Run() {
   new TGMWDCConfig;
   return 1;
}
