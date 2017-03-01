/**
 * @file   TCmdFileCd.h
 * @brief  change directory to file root
 *
 * @date   Created       : 2016-02-03 15:41:46 JST
 *         Last Modified : 2016-02-03 16:01:13 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_A076A8E8_90AC_4CBD_87BA_8E529DB1D628
#define INCLUDE_GUARD_UUID_A076A8E8_90AC_4CBD_87BA_8E529DB1D628

#include "TCatCmd.h"

namespace art {
   class TCmdFileCd;
}

class art::TCmdFileCd : public TCatCmd {
public:
   TCmdFileCd();
   virtual ~TCmdFileCd();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(UInt_t id, Option_t *opt = "") const;

   void Help();

private:
   TCmdFileCd(const TCmdFileCd&); // undefined
   TCmdFileCd& operator=(const TCmdFileCd&); // undefined

   ClassDef(TCmdFileCd,1) // change directory to file root
};

#endif // INCLUDE_GUARD_UUID_A076A8E8_90AC_4CBD_87BA_8E529DB1D628
