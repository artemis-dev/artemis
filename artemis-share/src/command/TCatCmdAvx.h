/* $Id:$ */
/**
 * @file   TCatCmdAvx.h
 * @date   Created : Feb 09, 2012 22:09:42 JST
 *   Last Modified : Feb 09, 2012 22:29:52 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDAVX_H
#define TCATCMDAVX_H

#include "TCatCmdAv.h"

class TCatCmdAvx  : public TCatCmdAv {
protected:
   TCatCmdAvx();
public:
   ~TCatCmdAvx();

   static TCatCmdAvx* Instance();

   ClassDef(TCatCmdAvx,1);
};
#endif // end of #ifdef TCATCMDAVX_H
