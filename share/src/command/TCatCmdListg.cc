/**
 * @file   TCatCmdListg.cc
 * @brief
 *
 * @date   Created:       2014-01-17 13:14:45
 *         Last Modified: 2014-01-17 15:02:21
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TCatCmdListg.h"

#include <TROOT.h>
#include <TTree.h>
#include <TList.h>

using art::TCatCmdListg;

ClassImp(art::TCatCmdListg)

// Default constructor
TCatCmdListg::TCatCmdListg() {
   SetName("listg ");
   SetTitle("show the list of gates");
}

TCatCmdListg::~TCatCmdListg() {
}

TCatCmdListg* TCatCmdListg::Instance() {
   static TCatCmdListg instance;
   return &instance;
}

Long_t TCatCmdListg::Cmd(vector<TString> tokens) {
   if (tokens.size() != 2) {
//      Help();
      printf("Usage: listg treename\n");
      return 1;
   }

   TString treename(tokens[1]);
   return Run(treename);
}

Long_t TCatCmdListg::Run(const TString& treename) {
   TTree *tree = dynamic_cast<TTree*>(gROOT->FindObject(treename));
   if (!tree) {
      printf("TTree named \"%s\" not found\n", treename.Data());
      return 1;
   }
   TList *userinfo = static_cast<TList*>(tree->GetUserInfo());
   TList *gateinfo = static_cast<TList*>(userinfo->FindObject("gateinfo"));

   printf("List of Gates in '%s'\n",treename.Data());
   printf("ID\tName\tDescription\n");
   gateinfo->R__FOR_EACH(TObject,Print)("listg");

   return 1;
}
