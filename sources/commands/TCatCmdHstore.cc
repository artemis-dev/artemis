/* $Id:$ */
/**
 * @file   TCatCmdHstore.cc
 * @date   Created : Jul 13, 2012 10:13:10 JST
 *   Last Modified : Aug 05, 2013 19:17:43 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHstore.h"

#include <TFile.h>

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
   TList *objects = gDirectory->GetList();
   TDirectory *wkdir = gDirectory;
   // open file
   TFile *file = TFile::Open(filename,opt);
   // if file is not opened or writable return normal
   if (file && file->IsWritable()) {
      WriteRecursive(file,objects);
   } else {
      printf(" File %s does not exist or is not writable\n",filename);
      printf(" Please check your option : %s\n",opt);
   }
   wkdir->cd();     
   return 1;
}

void TCatCmdHstore::WriteRecursive(TDirectory *parent, TList *list)
{
   if (!parent || !list) return;
   Int_t nObj = list->GetEntries();
   for (Int_t i=0; i != nObj; i++) {
      TObject *obj = list->At(i);
      parent->cd();
      if (obj->InheritsFrom("TH1")) {
         obj->Write();
      } else if (obj->InheritsFrom("TDirectory")) {
         TDirectory *dir    = (TDirectory*)obj;
         TDirectory *newdir = parent->mkdir(dir->GetName());
         newdir->SetTitle(dir->GetTitle());
         WriteRecursive(newdir,dir->GetList());
      }
   }
}
