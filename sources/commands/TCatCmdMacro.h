/* $Id:$ */
/**
 * @file   TCatCmdMacro.h
 * @date   Created : Jul 27, 2012 18:27:37 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDMACRO_H
#define TCATCMDMACRO_H

#include "TCatCmd.h"

class TCatCmdMacro  : public TCatCmd{

public:
   TCatCmdMacro();
   ~TCatCmdMacro();

   static TCatCmdMacro *Instance();
   Long_t Cmd(vector<TString> tokens);
};
#endif // end of #ifdef TCATCMDMACRO_H
