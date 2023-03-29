/* $Id:$ */
/**
 * @file   TCmdXsta.h
 * @date   Created : 2015-03-29 16:07:53 JST
 *   Last Modified : 2015-04-08 17:41:51 JST by tokieda
 * @author Hiroshi Tokieda <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#ifndef TCMDXSTA_H
#define TCMDXSTA_H

#include "TPad.h"
#include <TCatCmd.h>

class TCmdXsta  : public TCatCmd {
protected:
   TCmdXsta();
   Double_t fX1;
   Double_t fY1;
   Double_t fX2;
   Double_t fY2;
   Double_t fE;
public:
   ~TCmdXsta();

   static TCmdXsta* Instance();
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TPad *pad = (TPad*) gPad, Double_t *x1=NULL, Double_t *y1=NULL, Double_t *x2=NULL, Double_t *y2=NULL); 
   Long_t Run(Double_t *x1, Double_t *y1, Double_t *x2, Double_t *y2); 
   void DrawLine(Double_t x, Double_t y);
   Double_t GetX1() const{return fX1;}
   Double_t GetY1() const{return fY1;}
   Double_t GetX2() const{return fX2;}
   Double_t GetY2() const{return fY2;}
   Double_t GetEntries() const{return fE;}
   void Help();

   ClassDef(TCmdXsta,1);
};
#endif // end of #ifdef TCMDXSTA_H
