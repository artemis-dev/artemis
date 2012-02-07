/* $Id:$ */
/**
 * @file   TCatCmdZone.cc
 * @date   Created : Feb 06, 2012 19:06:47 JST
 *   Last Modified : Feb 06, 2012 20:12:22 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdZone.h"

#include <TCatPadManager.h>

TCatCmdZone::TCatCmdZone()
{
   SetName("zone");
   SetTitle("divide canvas");
}
TCatCmdZone::~TCatCmdZone()
{
}

TCatCmdZone* TCatCmdZone::Instance()
{
   static TCatCmdZone instance;
   return &instance;
}

Long_t TCatCmdZone::Cmd(vector<TString> tokens)
{
   Int_t nx = 1;
   Int_t ny = 1;
   Float_t xmargin = 0.01;
   Float_t ymargin = 0.01;

   for (size_t i=1; i<tokens.size();i++) {
      switch (i) {
      case 1:
         nx = tokens[i].Atoi();
         break;
      case 2:
         ny = tokens[i].Atoi();
         break;
      case 3:
         xmargin = tokens[i].Atof();
         break;
      case 4:
         ymargin = tokens[i].Atof();
         break;
      }
   }
   return Run(nx,ny,xmargin,ymargin);
}

Long_t TCatCmdZone::Run(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin)
{
   TCatPadManager::Instance()->Divide(nx,ny,xmargin,ymargin);
   return 1;
}
