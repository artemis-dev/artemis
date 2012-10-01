/* $Id:$ */
/**
 * @file   TCatCmdSave.h
 * @date   Created : Oct 01, 2012 13:01:07 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDSAVE_H
#define TCATCMDSAVE_H

#include <TCatCmd.h>

class TCatCmdSave  : public TCatCmd {

public:
   TCatCmdSave();
   ~TCatCmdSave();

   static TCatCmdSave* Instance();
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TString &filename);

   ClassDef(TCatCmdSave,1);

};
#endif // end of #ifdef TCATCMDSAVE_H
