/* $Id:$ */
/**
 * @file   TCatCmdHb.h
 * @date   Created : Feb 06, 2012 20:06:18 JST
 *   Last Modified : Feb 09, 2012 11:38:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHB_H
#define TCATCMDHB_H

#include <TCatCmd.h>

class TCatCmdHb : public TCatCmd {
protected:
   TCatCmdHb();
public:
   ~TCatCmdHb();
   static TCatCmdHb* Instance();

   virtual Long_t Cmd(vector<TString> args);
   
   virtual Long_t Run(TString &opt);

   ClassDef(TCatCmdHb,1);
};
#endif // end of #ifdef TCATCMDHB_H
