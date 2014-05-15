/* $Id:$ */
/**
 * @file   TCatCmdMnx.h
 * @date   Created : Feb 09, 2012 22:09:42 JST
 *   Last Modified : Feb 09, 2012 22:29:52 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDMNX_H
#define TCATCMDMNX_H

#include "TCatCmdMn.h"

class TCatCmdMnx  : public TCatCmdMn {
protected:
   TCatCmdMnx();
public:
   ~TCatCmdMnx();

   static TCatCmdMnx* Instance();

   ClassDef(TCatCmdMnx,1);
};
#endif // end of #ifdef TCATCMDMNX_H
