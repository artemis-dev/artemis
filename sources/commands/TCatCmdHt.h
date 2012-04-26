/* $Id:$ */
/**
 * @file   TCatCmdHt.h
 * @date   Created : Feb 06, 2012 11:06:49 JST
 *   Last Modified : Feb 19, 2012 14:35:22 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHT_H
#define TCATCMDHT_H

#include <TCatCmd.h>


class TCatCmdHt  : public TCatCmd {
protected:
   TCatCmdHt();
public:
   ~TCatCmdHt();
   static TCatCmdHt* Instance();

   virtual Long_t Cmd(vector<TString> args);

   virtual Long_t Run(Int_t id1, Int_t id2, TString& opt);
   
   ClassDef(TCatCmdHt,1);
};
#endif // end of #ifdef TCATCMDHT_H
