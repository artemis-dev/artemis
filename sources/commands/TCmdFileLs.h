/**
 * @file   TCmdFileLs.h
 * @brief  show file list
 *
 * @date   Created       : 2016-02-02 18:12:13 JST
 *         Last Modified : 2016-02-02 18:13:36 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_F244980B_DA92_4035_A773_FA8519E2CDF7
#define INCLUDE_GUARD_UUID_F244980B_DA92_4035_A773_FA8519E2CDF7

#include "TCatCmd.h"

namespace art {
   class TCmdFileLs;
}

class art::TCmdFileLs : public TCatCmd {
public:
   TCmdFileLs();
   virtual ~TCmdFileLs();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(Option_t *opt = "") const;

   void Help();

private:
   TCmdFileLs(const TCmdFileLs&); // undefined
   TCmdFileLs& operator=(const TCmdFileLs& rhs); // undefined

   ClassDef(TCmdFileLs,1) // show file list
};

#endif // INCLUDE_GUARD_UUID_F244980B_DA92_4035_A773_FA8519E2CDF7
