/**
 * @file   TCmdFileCd.cc
 * @brief  change directory to file root
 *
 * @date   Created       : 2016-02-03 15:53:48 JST
 *         Last Modified : 2016-02-03 18:33:44 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TCmdFileCd.h"

#include <TROOT.h>
#include <TList.h>
#include <TFile.h>
#include <TClass.h>

using art::TCmdFileCd;

ClassImp(TCmdFileCd)

TCmdFileCd::TCmdFileCd()
{
   SetName("fcd");
   SetTitle("change directory to file root");
}

TCmdFileCd::~TCmdFileCd()
{
}

Long_t TCmdFileCd::Cmd(vector<TString> tokens)
{
   if (tokens.size() < 2) {
      Help();
      return 1;
   }

   if (!tokens[1].IsDigit()) {
      Error("Cmd","Invalid id input: '%s'",tokens[1].Data());
      Help();
      return 1;
   }

   const UInt_t id = tokens[1].Atoi();
   Run(id);
   return 1;
}

Long_t TCmdFileCd::Run(UInt_t id, Option_t*) const
{
   TList *const list = static_cast<TList*>(gROOT->GetListOfFiles());
   const UInt_t size = list->GetSize();
   if (!size) {
      Info("Run","no file found.\n");
      return 1;
   }

   if ( size <= id ) {
      printf("invalid id.\n");
      return 1;
   }

   if (TFile *file = dynamic_cast<TFile*>(list->At(id))) {
      file->cd();
   }

   return 1;
}

void TCmdFileCd::Help()
{
   printf("Usage   : %s id\n",GetName());
}
