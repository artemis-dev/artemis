/* $Id:$ */
/**
 * @file   TCatCmdLoopTerminate.h
 * @date   Created : Sep 16, 2013 12:16:38 JST
 *   Last Modified : Sep 16, 2013 12:23:38 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATCMDLOOPTERMINATE_H
#define TCATCMDLOOPTERMINATE_H

#include "TCatCmd.h"

class TCatCmdLoopTerminate  : public TCatCmd {
protected:
   TCatCmdLoopTerminate();
public:
   ~TCatCmdLoopTerminate();

   static TCatCmdLoopTerminate* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopTerminate,1);

};
#endif // end of #ifdef TCATCMDLOOPTERMINATE_H
