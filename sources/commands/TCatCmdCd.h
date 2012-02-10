/* $Id:$ */
/**
 * @file   TCatCmdCd.h
 * @date   Created : Feb 09, 2012 22:09:35 JST
 *   Last Modified : Feb 09, 2012 22:42:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDCD_H
#define TCATCMDCD_H

#include <TCatCmd.h>

class TCatCmdCd  : public TCatCmd {
protected:
   TCatCmdCd();
public:
   ~TCatCmdCd();

   static TCatCmdCd* Instance();

   virtual Long_t Cmd(vector<TString> args);
//   virtual Long_t Run(TString dir);
//   virtual Long_t Run(const Int_t &id);

   ClassDef(TCatCmdCd,1);

};
#endif // end of #ifdef TCATCMDCD_H
