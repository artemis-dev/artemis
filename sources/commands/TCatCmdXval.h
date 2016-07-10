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

class TPad;

#include <TCatCmd.h>

class TCatCmdXval  : public TCatCmd {
protected:
   TCatCmdXval();
   Double_t fX;
   Double_t fY;
public:
   ~TCatCmdXval();

   static TCatCmdXval* Instance();
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TPad *pad, Double_t *x=NULL, Double_t *y=NULL);
   Long_t Run(Double_t *x=NULL, Double_t *y=NULL);
   void GetEvent();
   Double_t GetX() const{return fX;}
   Double_t GetY() const{return fY;}

   ClassDef(TCatCmdXval,1);
};
#endif // end of #ifdef TCATCMDXVAL_H
