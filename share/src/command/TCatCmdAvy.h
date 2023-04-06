/* $Id:$ */
/**
 * @file   TCatCmdAvy.h
 * @date   Created : Feb 09, 2012 22:09:32 JST
 *   Last Modified : Feb 09, 2012 22:29:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDAVY_H
#define TCATCMDAVY_H

#include "TCatCmdAv.h"

class TCatCmdAvy  : public TCatCmdAv {
protected:
   TCatCmdAvy();
public:
   ~TCatCmdAvy();

   static TCatCmdAvy* Instance();

   ClassDef(TCatCmdAvy,1);
};
#endif // end of #ifdef TCATCMDAVY_H
