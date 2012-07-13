/* $Id:$ */
/**
 * @file   TCatCmdHstore.h
 * @date   Created : Jul 13, 2012 10:13:54 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDHSTORE_H
#define TCATCMDHSTORE_H

#include <TCatCmd.h>

class TDirectory;
class TList;

class TCatCmdHstore  : public TCatCmd {

public:
   TCatCmdHstore();
   ~TCatCmdHstore();

   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(const char *filename, const Option_t* = "");

protected:
   virtual void WriteRecursive(TDirectory *parent, TList *list);
   
   ClassDef(TCatCmdHstore,1);
};
#endif // end of #ifdef TCATCMDHSTORE_H
