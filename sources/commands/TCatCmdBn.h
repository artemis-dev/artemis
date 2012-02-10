/* $Id:$ */
/**
 * @file   TCatCmdBn.h
 * @date   Created : Feb 09, 2012 10:09:53 JST
 *   Last Modified : Feb 09, 2012 11:40:40 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDBN_H
#define TCATCMDBN_H

#include <TCatCmd.h>

class TH1;
class TH2;

class TCatCmdBn  : public TCatCmd {
public: 
   typedef enum { kX, kY, kZ } EAxis;
protected:
   TCatCmdBn(EAxis axis);
public:
   virtual ~TCatCmdBn();
   virtual Long_t Cmd(vector<TString> args);
   virtual TH1* Run(TH2* h2, Float_t start, Float_t end);

protected:
   EAxis fAxis;

   ClassDef(TCatCmdBn,1);
};
#endif // end of #ifdef TCATCMDBN_H
