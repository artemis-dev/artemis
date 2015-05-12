/**
 * @file   TCmdHdel.h
 * @brief  delete objects in the current directory
 *
 * @date   Created       : 2015-05-06 10:43:31 JST
 *         Last Modified : May 06, 2015 10:45:10 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_8B46B30A_C2B7_4A1A_888C_24949851E6E9
#define INCLUDE_GUARD_UUID_8B46B30A_C2B7_4A1A_888C_24949851E6E9

#include "TCatCmd.h"

namespace art {
   class TCmdHdel;
}

class art::TCmdHdel : public TCatCmd {
public:
   TCmdHdel();
   virtual ~TCmdHdel();

   TCmdHdel(const TCmdHdel& rhs);
   TCmdHdel& operator=(const TCmdHdel& rhs);

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(Int_t id1, Int_t id2);
   

protected:

private:

   ClassDef(TCmdHdel,1) // delete objects in the current directory
};

#endif // INCLUDE_GUARD_UUID_8B46B30A_C2B7_4A1A_888C_24949851E6E9
