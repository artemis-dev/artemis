/* $Id:$ */
/**
 * @file   TCatCmdBnx.h
 * @date   Created : Feb 09, 2012 11:09:42 JST
 *   Last Modified : Feb 09, 2012 19:24:44 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDBNX_H
#define TCATCMDBNX_H

#include <TCatCmdBn.h>

class TCatCmdBnx  : public TCatCmdBn {
protected:
   TCatCmdBnx();
public:
   virtual ~TCatCmdBnx();
   
   static TCatCmdBnx* Instance() { 
      static TCatCmdBnx instance;
      return &instance;
   }

   ClassDef(TCatCmdBnx,1);
            
};
#endif // end of #ifdef TCATCMDBNX_H
