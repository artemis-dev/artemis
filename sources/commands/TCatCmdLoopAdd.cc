/* $Id:$ */
/**
 * @file   TCatCmdLoopAdd.cc
 * @date   Created : May 02, 2012 16:02:35 JST
 *   Last Modified : Sep 16, 2013 11:09:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopAdd.h"
#include "TLoopManager.h"
#include <TROOT.h>


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
   if (args.size()!=2) {
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

   art::TLoopManager *man = art::TLoopManager::Instance();
   man->Add(filename);

   return 1;
}

void TCatCmdLoopAdd::Help()
{
}





