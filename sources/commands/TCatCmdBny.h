/* $Id:$ */
/**
 * @file   TCatCmdBny.h
 * @date   Created : Feb 09, 2012 19:09:53 JST
 *   Last Modified : Feb 09, 2012 19:28:54 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDBNY_H
#define TCATCMDBNY_H

#include <TCatCmdBn.h>

class TCatCmdBny  : public TCatCmdBn {
protected:
   TCatCmdBny();
public:
   ~TCatCmdBny();

   static TCatCmdBny* Instance();

   ClassDef(TCatCmdBny,1);
};
#endif // end of #ifdef TCATCMDBNY_H
