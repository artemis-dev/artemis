/**
 * @file   TCmdPcd.cc
 * @brief  pad cd
 *
 * @date   Created       : 2017-06-17 23:58:51 JST
 *         Last Modified : 2017-06-17 23:59:06 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdPcd.h"

#include <TCatPadManager.h>

using art::TCmdPcd;

ClassImp(TCmdPcd)

TCmdPcd::TCmdPcd()
{
   SetName("pcd");
   SetTitle("select pad");
}

TCmdPcd::~TCmdPcd()
{
}

TCmdPcd *TCmdPcd::Instance()
{
   static TCmdPcd instance;
   return &instance;
}

Long_t TCmdPcd::Cmd(vector<TString> tokens)
{
   if (tokens.size() == 1) {
      return Run();
   } if (tokens.size() > 2 || !tokens[1].IsDec()) {
      Help();
      return 1;
   }

   const Int_t id = tokens[1].Atoi();
   return Run(id);
}

Long_t TCmdPcd::Run(Int_t i)
{
   if (i<0) {
      Error("Run","pad_id should be non-negative integer.");
      return 1;
   }
   
   TCatPadManager *const pman = TCatPadManager::Instance();
   if (!pman->HasChild()) {
      Error("Run","there is no subpads.");
      return 1;
   }
   
   const Int_t n = pman->GetNumChild();
   if (n < i) {
      Error("Run","given id (%d) is larger than the max id of subpads (%d)",
            i,n);
      return 1;
   }
   pman->Get(i)->cd();
   pman->SetCurrentPadId(i);
   return 1;
}

void TCmdPcd::Help()
{
   printf("usage: %s pad_id\n",GetName());
}
