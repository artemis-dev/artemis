/* $Id:$ */
/**
 * @file   TCatCmdLoopAdd.h
 * @date   Created : May 02, 2012 16:02:46 JST
 *   Last Modified : May 02, 2012 16:31:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDLOOPADD_H
#define TCATCMDLOOPADD_H

#include <TCatCmd.h>

class TCatCmdLoopAdd  : public TCatCmd {
protected:
   TCatCmdLoopAdd();
public:
   ~TCatCmdLoopAdd();

   static TCatCmdLoopAdd* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopAdd,1);
};
#endif // end of #ifdef TCATCMDLOOPADD_H
