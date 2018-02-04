/**
 * @file   TCmdPb.h
 * @brief  select previous subpad
 *
 * @date   Created       : 2017-06-18 22:02:54 JST
 *         Last Modified : 2017-06-18 22:02:56 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_00C1BBE4_07CF_47E7_9734_42FACBE94763
#define INCLUDE_GUARD_UUID_00C1BBE4_07CF_47E7_9734_42FACBE94763

#include "TCatCmd.h"

namespace art {
   class TCmdPb;
}

class art::TCmdPb : public TCatCmd {
public:
   static TCmdPb* Instance();
   virtual ~TCmdPb();

   Long_t Cmd(vector<TString>);
   Long_t Run();
   void Help();
   
protected:

private:
   TCmdPb();
   TCmdPb(const TCmdPb&); // prohibited
   TCmdPb& operator=(const TCmdPb& rhs); // prohibited
   
   ClassDef(TCmdPb,1) // select previous subpad
};

#endif // INCLUDE_GUARD_UUID_00C1BBE4_07CF_47E7_9734_42FACBE94763
