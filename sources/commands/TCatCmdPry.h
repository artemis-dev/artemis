/* $Id:$ */
/**
 * @file   TCatCmdPry.h
 * @date   Created : Feb 09, 2012 22:09:32 JST
 *   Last Modified : Feb 09, 2012 22:29:59 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDPRY_H
#define TCATCMDPRY_H

#include <TCatCmdPr.h>

class TCatCmdPry  : public TCatCmdPr {
protected:
   TCatCmdPry();
public:
   ~TCatCmdPry();

   static TCatCmdPry* Instance();

   ClassDef(TCatCmdPry,1);
};
#endif // end of #ifdef TCATCMDPRY_H
