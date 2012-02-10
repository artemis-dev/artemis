/* $Id:$ */
/**
 * @file   TCatCmdHelp.h
 * @date   Created : Feb 10, 2012 20:10:52 JST
 *   Last Modified : Feb 10, 2012 20:48:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHELP_H
#define TCATCMDHELP_H

#include <TCatCmd.h>

class TCatCmdHelp  : public TCatCmd {
protected:
   TCatCmdHelp();
public:
   ~TCatCmdHelp();

   static TCatCmdHelp* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdHelp,1);
};
#endif // end of #ifdef TCATCMDHELP_H
