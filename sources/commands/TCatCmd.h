/* $Id:$ */
/**
 * @file   TCatCmd.h
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : Feb 10, 2012 20:19:56 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMD_H
#define TCATCMD_H

#include <TNamed.h>
#include <vector>
using namespace std;

class TCatCmd : public TNamed{
protected:
   TCatCmd();
public:
   ~TCatCmd();
   
   virtual Long_t Exec(TString &line);

   virtual Long_t Cmd(vector<TString> args) = 0;

   virtual void GetRange(TString &arg, Int_t &id1, Int_t &id2, TString delim = ":");

   virtual Bool_t IsSortable() const { return kTRUE; }

   virtual Int_t Compare(const TObject *obj) const ;

   virtual void Help();

};
#endif // end of #ifdef TCATCMD_H
