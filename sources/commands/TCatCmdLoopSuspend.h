/* $Id:$ */
/**
 * @file   TCatCmdLoopSuspend.h
 * @date   Created : May 02, 2012 16:02:31 JST
 *   Last Modified : May 02, 2012 16:19:33 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDLOOPSUSPEND_H
#define TCATCMDLOOPSUSPEND_H

#include "TCatCmd.h"

#include "TCatCmd.h"

class TCatCmdLoopSuspend  : public TCatCmd {
protected:
   TCatCmdLoopSuspend();
public:
   ~TCatCmdLoopSuspend();

   static TCatCmdLoopSuspend* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopSuspend,1); // 
};
#endif // end of #ifdef TCATCMDLOOPSUSPEND_H
