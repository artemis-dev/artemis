/* $Id:$ */
/**
 * @file   TCatCmdLg.h
 * @date   Created : Jun 20, 2013 04:20:44 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATCMDLG_H
#define TCATCMDLG_H

#include <TCatCmd.h>
#include <TMethodCall.h>

class TCatCmdLg  : public TCatCmd {

public:
   typedef enum { kX, kY, kZ } EAxis;

   TCatCmdLg(const EAxis axis, Bool_t value);
   ~TCatCmdLg();

   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(Int_t padid1 = -1, Int_t padid2 = -1);
private:
   EAxis  fAxis; // axis
   Bool_t fValue; // value to be set in SetLogX etc. 0: linear, 1: log
   TMethodCall *fMethod;

   ClassDef(TCatCmdLg,1);
};
#endif // end of #ifdef TCATCMDLG_H
