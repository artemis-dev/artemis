/**
 * @file   TCmdMWDCConfig.h
 * @brief
 *
 * @date   Created:       2014-01-16 14:19:54
 *         Last Modified: 2014-07-10 17:31:49 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TCMDMWDCCONFIG_H
#define TCMDMWDCCONFIG_H

#include <TCatCmd.h>

namespace art {
   class TCmdMWDCConfig;
   class TGMWDCConfig;
}

class art::TCmdMWDCConfig : public TCatCmd {
public:
   virtual ~TCmdMWDCConfig();

   virtual Long_t Cmd(vector<TString>);
   virtual Long_t Run();

   static TCmdMWDCConfig* Instance();

protected:
   TCmdMWDCConfig();
private:
   TCmdMWDCConfig(const TCmdMWDCConfig&);
   TCmdMWDCConfig& operator=(const TCmdMWDCConfig&);

   ClassDef(TCmdMWDCConfig,0)
};

#endif // TCMDMWDCCONFIG_H
