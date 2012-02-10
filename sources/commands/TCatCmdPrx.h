/* $Id:$ */
/**
 * @file   TCatCmdPrx.h
 * @date   Created : Feb 09, 2012 22:09:42 JST
 *   Last Modified : Feb 09, 2012 22:29:52 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDPRX_H
#define TCATCMDPRX_H

#include <TCatCmdPr.h>

class TCatCmdPrx  : public TCatCmdPr {
protected:
   TCatCmdPrx();
public:
   ~TCatCmdPrx();

   static TCatCmdPrx* Instance();

   ClassDef(TCatCmdPrx,1);
};
#endif // end of #ifdef TCATCMDPRX_H
