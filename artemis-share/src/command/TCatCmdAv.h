/* $Id:$ */
/**
 * @file   TCatCmdAv.h
 * @date   Created : Feb 07, 2012 19:07:59 JST
 *   Last Modified : Feb 09, 2012 22:24:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C) 2012
 */
#ifndef TCATCMDAV_H
#define TCATCMDAV_H

#include <TCatCmd.h>

class TH1;
class TH2;
class TProfile;
class TCatCmdAv  : public TCatCmd {
public:
   typedef enum { kX, kY, kZ } EAxis;
protected:
   TCatCmdAv(const EAxis& axis);
public:
   virtual ~TCatCmdAv();

   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(Int_t id1, Int_t id2 = -1);
   virtual void* Run(TH2* h2);

protected:
   EAxis fAxis;

   ClassDef(TCatCmdAv,1);
};
#endif // end of #ifdef TCATCMDAV_H
