/**
 * @file   TCmdFipo.h
 * @brief  fit by pol functions
 *
 * @date   Created       : 2016-01-15 15:04:47 JST
 *         Last Modified : 2016-01-18 12:12:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_AA911453_AD27_46AD_BE4E_CF7E13E292EF
#define INCLUDE_GUARD_UUID_AA911453_AD27_46AD_BE4E_CF7E13E292EF

#include "TCatCmd.h"

namespace art {
   class TCmdFipo;
}

class TH1;

class art::TCmdFipo : public TCatCmd {
public:
   TCmdFipo();
   virtual ~TCmdFipo();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TH1 *h, UInt_t degree = 1, Double_t *params = NULL) const;
   void Help();

private:
   TCmdFipo(const TCmdFipo&); // undefined
   TCmdFipo& operator=(const TCmdFipo&); // undefined

   ClassDef(TCmdFipo,1) // fit by pol functions
};

#endif // INCLUDE_GUARD_UUID_AA911453_AD27_46AD_BE4E_CF7E13E292EF
