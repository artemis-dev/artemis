/* $Id:$ */
/**
 * @file   TCatCmdFactory.h
 * @date   Created : Feb 06, 2012 10:06:51 JST
 *   Last Modified : Feb 06, 2012 11:03:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDFACTORY_H
#define TCATCMDFACTORY_H

#include <TNamed.h>
#include <TCatCmd.h>

class TCatCmdFactory  : public TNamed {

protected:
   TCatCmdFactory();

public:
   ~TCatCmdFactory();

   static TCatCmdFactory* Instance();

   virtual Long_t ProcessLine(TString line);

   virtual void Register(TCatCmd *cmd);

private:
   TObjArray *fCmds;

   
   ClassDef(TCatCmdFactory,1);
};
#endif // end of #ifdef TCATCMDFACTORY_H
