/* $Id:$ */
/**
 * @file   TCatCmdHn.h
 * @date   Created : Feb 06, 2012 20:06:18 JST
 *   Last Modified : Feb 09, 2012 11:38:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHN_H
#define TCATCMDHN_H

#include <TCatCmd.h>

class TCatCmdHn  : public TCatCmd {
protected:
   TCatCmdHn();
public:
   ~TCatCmdHn();
   static TCatCmdHn* Instance();

   virtual Long_t Cmd(vector<TString> args);
   
   virtual Long_t Run(TString &opt);

   ClassDef(TCatCmdHn,1);
};
#endif // end of #ifdef TCATCMDHN_H
