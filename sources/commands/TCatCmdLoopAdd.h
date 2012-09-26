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

class TFolder;

class TCatCmdLoopAdd  : public TCatCmd {
protected:
   TCatCmdLoopAdd();
public:
   ~TCatCmdLoopAdd();

   static TCatCmdLoopAdd* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual void Help();

   // load yaml-format steering file
   // this method should be moved to elsewhere
   virtual Bool_t LoadYAML(TString filename, TFolder *thisloop);

   ClassDef(TCatCmdLoopAdd,1);
};
#endif // end of #ifdef TCATCMDLOOPADD_H
