/* $Id:$ */
/**
 * @file   TCatCmdFactory.h
 * @date   Created : Feb 06, 2012 10:06:51 JST
 *   Last Modified : Feb 10, 2012 20:12:27 JST
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

   virtual void Help(TString cmd = "");

   virtual void SetOptExactName(Bool_t flag) { fFlagExactName = flag; }

private:
   TObjArray *fCmds;

   Bool_t fFlagExactName;

   
   ClassDef(TCatCmdFactory,1);
};
#endif // end of #ifdef TCATCMDFACTORY_H
