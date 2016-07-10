/* $Id:$ */
/**
 * @file   TCatCmdPr.h
 * @date   Created : Feb 07, 2012 19:07:59 JST
 *   Last Modified : 2016-01-18 13:53:27 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDPR_H
#define TCATCMDPR_H

#include <TCatCmd.h>

class TH1;
class TH2;

class TCatCmdPr  : public TCatCmd {
public:
   typedef enum { kX, kY, kZ } EAxis;
   TCatCmdPr(const EAxis& axis, Bool_t isProjection = kTRUE);
   virtual ~TCatCmdPr();

   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(Int_t id1, Int_t id2 = -1, Option_t *opt = "");
   virtual TH1* Run(TH2* h2,Option_t *opt = "");

protected:
   EAxis fAxis;
   Char_t fAxisLabel;
   Bool_t fIsProjection;

   ClassDef(TCatCmdPr,1);
};
#endif // end of #ifdef TCATCMDPR_H
