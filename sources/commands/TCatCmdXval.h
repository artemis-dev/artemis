/* $Id:$ */
/**
 * @file   TCatCmdXval.h
 * @date   Created : Feb 03, 2013
 * @author Kawase Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATCMDXVAL_H
#define TCATCMDXVAL_H

#include <TCatCmd.h>

class TCatCmdXval  : public TCatCmd {
protected:
   TCatCmdXval();
public:
   ~TCatCmdXval();

   static TCatCmdXval* Instance();
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(Double_t& x, Double_t& y);
   void GetEvent();

   ClassDef(TCatCmdXval,1);
};
#endif // end of #ifdef TCATCMDXVAL_H
