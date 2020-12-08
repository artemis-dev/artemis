/* $Id:$ */
/**
 * @file   TCatCmdLoopAdd.cc
 * @date   Created : May 02, 2012 16:02:35 JST
 *   Last Modified : 2020-11-27 00:20:58 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopAdd.h"
#include "TLoopManager.h"
#include <TROOT.h>
#include <map>
#include "TObjString.h"

#include <fstream>


using namespace std;


ClassImp(TCatCmdLoopAdd);

TCatCmdLoopAdd::TCatCmdLoopAdd()
{
   SetName("add");
   SetTitle("Add a new loop to art::TLoopManager");
}
TCatCmdLoopAdd::~TCatCmdLoopAdd()
{
}

TCatCmdLoopAdd* TCatCmdLoopAdd::Instance()
{
   static TCatCmdLoopAdd instance;
   return &instance;
}

Long_t TCatCmdLoopAdd::Cmd(vector<TString> args)
{
   if (args.size() < 2) {
      Help();
      return 1;
   }
   TString &filename = args[1];
   // check if file exists
   ifstream fin(filename);
   if (!fin) {
      printf("Catnnot open %s\n",filename.Data());
      return 1;
   }
   fin.close();
   std::map<std::string, std::string> replace;
   for (Int_t i = 2, n = args.size(); i < n; ++i) {
      if (args[i].Contains('=')) {
         TObjArray *tokens = args[i].Tokenize('=');
         if (tokens->GetEntriesFast() != 2) {
            Error("Cmd",Form("replace pair contains more than one '=' : %s",args[i].Data()));
            return 1;
         }
         std::string key(((TObjString*)tokens->At(0))->String().Data());
         std::string value(((TObjString*)tokens->At(1))->String().Data());
         replace.insert(std::map<std::string,std::string>::value_type(key,value));
      } else {
         if (!args[i].IsDigit()) {
            Error("Cmd",Form("NUM replacement is given but contains non-digits : %s",args[i].Data()));
            return 1;
         }
         std::string key("NUM");
         std::string value(args[i].Data());
         replace.insert(std::map<std::string,std::string>::value_type(key,value));
      }
   }

   art::TLoopManager *man = art::TLoopManager::Instance();
   man->Add(filename,&replace);

   if (replace.size() > 0) {
      Info("Cmd","replacement is given");
   }



   return 1;
}

void TCatCmdLoopAdd::Help()
{
   TString help = "";
   Info("Help","NAME");
   Info("Help"," ");
   Info("Help","     add -- prepare event loop from steering file");
   Info("Help","SYNOPSIS");
   Info("Help","     add [file] [key1=val1] [num]");
   Info("Help"," ");
   Info("Help","DESCRIPTION");
   Info("Help","     The command 'add' prepare event loop from steering file. ");
   Info("Help","     Steering filename is mandatory as the first argument.");
   Info("Help","     If any pairs of the key and value is given, the keyword @key@");
   Info("Help","     in main steering file will be replaced by the value.");
   Info("Help","     Single value consisting only of digits will be replaced with @NUM@.");
   Info("Help","     This will be useful for changing the runnumber.");
}





