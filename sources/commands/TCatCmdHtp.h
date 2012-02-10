/* $Id:$ */
/**
 * @file   TCatCmdHtp.h
 * @date   Created : Feb 07, 2012 18:07:44 JST
 *   Last Modified : Feb 09, 2012 11:38:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHTP_H
#define TCATCMDHTP_H

#include <TCatCmd.h>

class TCatCmdHtp  : public TCatCmd {
protected:
   TCatCmdHtp();
public:
   ~TCatCmdHtp();
   static TCatCmdHtp *Instance();
   
   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(TString &opt);

   ClassDef(TCatCmdHtp,1);
};
#endif // end of #ifdef TCATCMDHTP_H
