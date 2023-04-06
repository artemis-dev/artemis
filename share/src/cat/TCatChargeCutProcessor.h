/* $Id:$ */
/**
 * @file   TCatChargeCutProcessor.h
 * @date   Created : Sep 29, 2012 20:29:52 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCHARGECUTPROCESSOR_H
#define TCATCHARGECUTPROCESSOR_H

#include "TCatSimpleCutProcessor.h"
#include <TROOT.h>

class TCatSimpleData;

class TCatChargeCutProcessor  : public TCatSimpleCutProcessor {

public:
   TCatChargeCutProcessor();
   virtual ~TCatChargeCutProcessor();

protected:
   virtual const char* GetInputClass() { return "TCatSimpleData"; }
   virtual Bool_t IsValid(const TCatSimpleData& data);


   ClassDef(TCatChargeCutProcessor,1);
};
#endif // end of #ifdef TCATCHARGECUTPROCESSOR_H
