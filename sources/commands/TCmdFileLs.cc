/**
 * @file   TCmdFileLs.cc
 * @brief  show file list
 *
 * @date   Created       : 2016-02-02 18:13:58 JST
 *         Last Modified : 2016-11-17 11:23:41 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TCmdFileLs.h"

#include <TROOT.h>
#include <TFile.h>
#include <TClass.h>

using art::TCmdFileLs;

ClassImp(TCmdFileLs)

TCmdFileLs::TCmdFileLs()
{
   SetName("fls");
   SetTitle("show file list");
}

TCmdFileLs::~TCmdFileLs()
{
}

Long_t TCmdFileLs::Cmd(vector<TString>)
{
   Run();
   return 1;
}

Long_t TCmdFileLs::Run(const char*) const
{
   TList *list = static_cast<TList*>(gROOT->GetListOfFiles());
   const UInt_t size = list->GetSize();
   if (!size) {
      Info("Run","no file found.");
      return 1;
   }

   printf(" files\n");
   TFile *const current_file = gDirectory->GetFile();

   for (UInt_t i = 0; i != size; ++i) {
      TObject *obj = list->At(i);
      if (const TFile *f = dynamic_cast<TFile*>(obj)) {
	 printf("%c%4d %4s %-15s %-10s (%s)\n",
		current_file==f ? '>' : ' ',
		i,f->IsA()->GetName(),f->GetName(),f->GetTitle(),f->GetOption());
      } else {
	 printf("%5d %4s %-15s %-10s\n",
		i,obj->IsA()->GetName(),obj->GetName(),obj->GetTitle());
      }
   }

   return 1;
}

void TCmdFileLs::Help()
{
   printf("Usage   : %s\n",GetName());
}
