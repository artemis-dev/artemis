/**
 * @file   TCmdPn.cc
 * @brief  select next subpad
 *
 * @date   Created       : 2017-06-18 22:18:50 JST
 *         Last Modified : 2017-06-18 22:18:52 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdPn.h"

#include <TCatPadManager.h>

using art::TCmdPn;

ClassImp(TCmdPn)

TCmdPn::TCmdPn()
{
   SetName("pn");
   SetTitle("select next subpad");
}

TCmdPn::~TCmdPn()
{
}

TCmdPn * TCmdPn::Instance()
{
   static TCmdPn instance;
   return &instance;
}

Long_t TCmdPn::Cmd(vector<TString>)
{
   return Run();
}

Long_t TCmdPn::Run()
{
   TCatPadManager::Instance()->Next();
   return 1;
}

void TCmdPn::Help()
{
   printf("usage: %s\n",GetName());
}
