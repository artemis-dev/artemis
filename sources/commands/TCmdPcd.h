/**
 * @file   TCmdPcd.h
 * @brief  pad cd
 *
 * @date   Created       : 2017-06-17 23:58:08 JST
 *         Last Modified : 2017-06-17 23:58:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_B626CD2F_A426_4454_AB21_9B2B63EBE2FC
#define INCLUDE_GUARD_UUID_B626CD2F_A426_4454_AB21_9B2B63EBE2FC

#include "TCatCmd.h"

namespace art {
   class TCmdPcd;
}

class art::TCmdPcd : public TCatCmd {
public:
   TCmdPcd();
   virtual ~TCmdPcd();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(Int_t i = 1);
   void Help();
   
protected:

private:
   TCmdPcd(const TCmdPcd&); // prohibited
   TCmdPcd& operator=(const TCmdPcd& ); // prohibited

   ClassDef(TCmdPcd,0) // pad cd
};

#endif // INCLUDE_GUARD_UUID_B626CD2F_A426_4454_AB21_9B2B63EBE2FC
