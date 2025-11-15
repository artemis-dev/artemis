/* $Id:$ */
/**
 * @file   TCatCmdHstore.cc
 * @date   Created : Jul 13, 2012 10:13:10 JST
 *   Last Modified : 2021-03-15 23:22:42 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHstore.h"
#include "TArtemisUtil.h"
#include <TFile.h>
#include <TH1FTreeProj.h>
#include <TH2FTreeProj.h>
#include <TH3FTreeProj.h>

TCatCmdHstore::TCatCmdHstore()
{
   SetName("hstore");
   SetTitle("store the histograms in the current directory");
}
TCatCmdHstore::~TCatCmdHstore()
{
}

Long_t TCatCmdHstore::Cmd(vector<TString> args)
{
   if (args.size() == 1) {
      // at least one arguments except command itself is required
      Help();
      return 1;
   }
   TString opt(args.size()>2 ? args[2] : "create");
   return Run(args[1],opt);
}


Long_t TCatCmdHstore::Run(const char* filename, const Option_t *opt)
{
   art::Util::PrepareDirectoryFor(filename);
   art::Util::WriteObjectsToFile(filename,opt);
   return 1;
}
void TCatCmdHstore::Help()
{
   printf(" hstore <filename> [option] : store histograms in the current directory to the file\n");
   printf("   option : same as TFile option (default is create)\n");
}