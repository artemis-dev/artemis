/* $Id:$ */
/**
 * @file   TCatCmdMn.h
 * @date   Created : Feb 07, 2012 19:07:59 JST
 *   Last Modified : Feb 09, 2012 22:24:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C) 2012
 */
#ifndef TCATCMDMN_H
#define TCATCMDMN_H

#include <TCatCmd.h>

class TH1;
class TH2;
class TProfile;
class TCatCmdMn  : public TCatCmd {
public:
   typedef enum { kX, kY, kZ } EAxis;
protected:
   TCatCmdMn(const EAxis& axis);
public:
   virtual ~TCatCmdMn();

   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(Int_t id1, Int_t id2 = -1);
   virtual TProfile* Run(TH2* h2);

protected:
   EAxis fAxis;

   ClassDef(TCatCmdMn,1);
};
#endif // end of #ifdef TCATCMDMN_H
