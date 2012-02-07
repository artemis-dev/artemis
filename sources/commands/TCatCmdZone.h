/* $Id:$ */
/**
 * @file   TCatCmdZone.h
 * @date   Created : Feb 06, 2012 19:06:04 JST
 *   Last Modified : Feb 06, 2012 19:45:45 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDZONE_H
#define TCATCMDZONE_H

#include <TCatCmd.h>

class TCatCmdZone  : public TCatCmd {
protected:
   TCatCmdZone();
public:
   ~TCatCmdZone();
   static TCatCmdZone *Instance();
   virtual Long_t Cmd(vector<TString> tokens);
   virtual Long_t Run(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin);
};
#endif // end of #ifdef TCATCMDZONE_H
