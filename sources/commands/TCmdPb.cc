/**
 * @file   TCmdPb.cc
 * @brief  select previous subpad
 *
 * @date   Created       : 2017-06-18 22:03:07 JST
 *         Last Modified : 2017-06-18 22:03:09 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdPb.h"

#include <TCatPadManager.h>

using art::TCmdPb;

ClassImp(TCmdPb)

TCmdPb::TCmdPb()
{
   SetName("pb");
   SetTitle("select previous subpad");
}

TCmdPb::~TCmdPb()
{
}

TCmdPb * TCmdPb::Instance()
{
   static TCmdPb instance;
   return &instance;
}

Long_t TCmdPb::Cmd(vector<TString>)
{
   return Run();
}

Long_t TCmdPb::Run()
{
   TCatPadManager::Instance()->Previous();
   return 1;
}

void TCmdPb::Help()
{
   printf("usage: %s\n",GetName());
}
