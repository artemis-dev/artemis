/* $Id:$ */
/**
 * @file   TCatCmd.h
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : Feb 06, 2012 19:42:55 JST
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

class TCatCmd  : public TNamed {
protected:
   TCatCmd();
public:
   ~TCatCmd();
   
   virtual Long_t Exec(TString &line);

   virtual Long_t Cmd(vector<TString> args) = 0;

   virtual void Help();
};
#endif // end of #ifdef TCATCMD_H
