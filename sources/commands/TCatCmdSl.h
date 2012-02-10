/* $Id:$ */
/**
 * @file   TCatCmdSl.h
 * @date   Created : Feb 09, 2012 23:09:45 JST
 *   Last Modified : Feb 10, 2012 00:28:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDSL_H
#define TCATCMDSL_H

#include <TCatCmd.h>

class TH1;
class TH2;

class TCatCmdSl  : public TCatCmd {
public:
   typedef enum { kX, kY } EAxis;
   TCatCmdSl(const EAxis& axis);
public:
   ~TCatCmdSl();

   virtual Long_t Cmd(vector<TString> args);
   virtual TH1* Run(TH2* h2);

protected:
   EAxis fAxis;

   ClassDef(TCatCmdSl,1);
};
#endif // end of #ifdef TCATCMDSL_H
