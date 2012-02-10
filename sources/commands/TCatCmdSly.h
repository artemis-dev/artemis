/* $Id:$ */
/**
 * @file   TCatCmdSly.h
 * @date   Created : Feb 10, 2012 00:10:41 JST
 *   Last Modified : Feb 10, 2012 00:31:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDSLY_H
#define TCATCMDSLY_H

#include <TCatCmdSl.h>

class TCatCmdSly  : public TCatCmdSl {
protected:
   TCatCmdSly();
public:
   ~TCatCmdSly();

   static TCatCmdSly* Instance();

   ClassDef(TCatCmdSl,1);
};
#endif // end of #ifdef TCATCMDSLY_H
