/* $Id:$ */
/**
 * @file   TCatCmdMny.h
 * @date   Created : Feb 09, 2012 22:09:32 JST
 *   Last Modified : Feb 09, 2012 22:29:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDMNY_H
#define TCATCMDMNY_H

#include "TCatCmdMn.h"

class TCatCmdMny  : public TCatCmdMn {
protected:
   TCatCmdMny();
public:
   ~TCatCmdMny();

   static TCatCmdMny* Instance();

   ClassDef(TCatCmdMny,1);
};
#endif // end of #ifdef TCATCMDAVY_H
