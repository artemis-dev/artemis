/* $Id:$ */
/**
 * @file   TCatCmdLs.h
 * @date   Created : Feb 07, 2012 12:07:37 JST
 *   Last Modified : Feb 09, 2012 22:45:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDLS_H
#define TCATCMDLS_H

#include <TCatCmd.h>

class TCatCmdLs  : public TCatCmd {
protected:
   TCatCmdLs();
public:
   ~TCatCmdLs();

   static TCatCmdLs* Instance();
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TString &opt);

   ClassDef(TCatCmdLs,1);
};
#endif // end of #ifdef TCATCMDLS_H
