/**
 * @file   TCatCmdListg.h
 * @brief
 *
 * @date   Created:       2014-01-16 14:19:54
 *         Last Modified: 2014-01-17 13:41:42
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TCATCMDLISTG_H
#define TCATCMDLISTG_H

#include <TCatCmd.h>

namespace art {
   class TCatCmdListg;
}

class art::TCatCmdListg : public TCatCmd {
public:
   virtual ~TCatCmdListg();

   virtual Long_t Cmd(vector<TString> tokens);
   virtual Long_t Run(const TString &treename);

   static TCatCmdListg* Instance();

protected:
   TCatCmdListg();
private:

   TCatCmdListg(const TCatCmdListg&);
   TCatCmdListg& operator=(const TCatCmdListg&);

   ClassDef(TCatCmdListg,0)
};

#endif // TCATCMDLISTG_H
