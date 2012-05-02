/* $Id:$ */
/**
 * @file   TCatCmdLoopResume.h
 * @date   Created : May 02, 2012 16:02:14 JST
 *   Last Modified : May 02, 2012 16:18:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDLOOPRESUME_H
#define TCATCMDLOOPRESUME_H

#include <TCatCmd.h>

class TCatCmdLoopResume  : public TCatCmd {
protected:
   TCatCmdLoopResume();
public:
   ~TCatCmdLoopResume();

   static TCatCmdLoopResume* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   ClassDef(TCatCmdLoopResume,1);

};
#endif // end of #ifdef TCATCMDLOOPRESUME_H
