/**
 * @file   TCmdPn.h
 * @brief  select next subpad
 *
 * @date   Created       : 2017-06-18 22:18:35 JST
 *         Last Modified : 2017-06-18 22:18:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_61967E2D_764D_47E3_8F02_2A02AE4439B8
#define INCLUDE_GUARD_UUID_61967E2D_764D_47E3_8F02_2A02AE4439B8

#include "TCatCmd.h"

namespace art {
   class TCmdPn;
}

class art::TCmdPn : public TCatCmd {
public:
   static TCmdPn* Instance();
   virtual ~TCmdPn();

   Long_t Cmd(vector<TString>);
   Long_t Run();
   void Help();

protected:

private:
   TCmdPn();
   TCmdPn(const TCmdPn&); // prohibited
   TCmdPn& operator=(const TCmdPn&); // prohibited

   ClassDef(TCmdPn,1) // select next subpad
};

#endif // INCLUDE_GUARD_UUID_61967E2D_764D_47E3_8F02_2A02AE4439B8
