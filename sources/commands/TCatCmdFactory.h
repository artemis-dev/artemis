/* $Id:$ */
/**
 * @file   TCatCmdFactory.h
 * @date   Created : Feb 06, 2012 10:06:51 JST
 *   Last Modified : 2016-08-17 17:16:48 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCMDFACTORY_H
#define TCATCMDFACTORY_H

#include <TCmdGroup.h>

class TCatCmdFactory  : public art::TCmdGroup {

protected:
   TCatCmdFactory();

public:
   ~TCatCmdFactory();

   static TCatCmdFactory* Instance();

   virtual Long_t ProcessLine(TString line);

   virtual void Help(TString cmd = "");


private:

   
   ClassDef(TCatCmdFactory,1);
};
#endif // end of #ifdef TCATCMDFACTORY_H
