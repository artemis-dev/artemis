/* $Id:$ */
/**
 * @file   TCatCmdSave.cc
 * @date   Created : Oct 01, 2012 13:01:21 JST
 *   Last Modified : 2017-01-18 17:04:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdSave.h"

#include "TCatPadManager.h"
#include <TSystem.h>
#include <TTimeStamp.h>
#include <TROOT.h>

TCatCmdSave::TCatCmdSave()
   : fDefaultDirectory(""), fHasDateFormat(kFALSE)
{
   SetName("save");
   SetTitle("save canvas to file");
}
TCatCmdSave::~TCatCmdSave()
{
}

TCatCmdSave* TCatCmdSave::Instance()
{
   static TCatCmdSave instance;
   return &instance;
}

Long_t TCatCmdSave::Cmd(vector<TString> tokens)
{
   if (tokens.size()>2) {
      Help();
      return 1;
   }
   TString filename = (tokens.size()==1)? "" : tokens[1];
   return Run(filename);
}

Long_t TCatCmdSave::Run(const char* filename)
{
   TString dir;
   TTimeStamp ts;
   if (GetAddDateDir()) {
      dir = fDefaultDirectory;
      dir += "/";
      // use non UTC date
      dir += ts.GetDate(kFALSE);
   } else if (HasDateFormat()) {
      // default directory name has date format
      dir = gSystem->GetFromPipe(TString::Format("date +%s",fDefaultDirectory.Data()));
   } else {
      dir = fDefaultDirectory;
   }

   gSystem->Exec(TString::Format("mkdir -p %s",dir.Data()));

   TString name = filename;
   std::vector<TString> files;
   if (GetAutoName()) {
      TVirtualPad *canvas = NULL;
      if (!name.IsNull()) {
         canvas = (TVirtualPad*)gROOT->GetListOfCanvases()->FindObject(name);
      } else {
         canvas = TCatPadManager::Instance()->GetCanvas();
      }
      if (!canvas) {
         Error("Run","canvas is not correctly given: '%s'",name.Data());
      } else {
         TString basename = TString::Format("%d",ts.GetSec());
         for (Int_t i=0; i!=fFormat.size(); i++) {
            TString f = TString::Format("%s/%s.%s",dir.Data(),basename.Data(),fFormat[i].Data());
            printf("%s\n",f.Data());
            canvas->SaveAs(f);
         }
      }
   } else if (!name.IsNull()) {
      TCatPadManager::Instance()->GetCanvas()->SaveAs(dir+filename);      
   } else {
      Error("Run","file name is not given");
   }
   return 1;
}
