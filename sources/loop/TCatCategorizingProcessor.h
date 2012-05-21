/* $Id:$ */
/**
 * @file   TCatCategorizingProcessor.h
 * @date   Created : May 13, 2012 18:13:49 JST
 *   Last Modified : May 21, 2012 11:25:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCATEGORIZINGPROCESSOR_H
#define TCATCATEGORIZINGPROCESSOR_H

#include <TCatProcessor.h>
#include <TCatEventCollection.h>

#include <TArtRawEventObject.h>
#include <TCatRawDataCategorized.h>


class TArtMapper;

class TCatCategorizingProcessor  : public TCatProcessor {

public:
   TCatCategorizingProcessor();
   ~TCatCategorizingProcessor();

   virtual void Init(TCatEventCollection *col);
   virtual void Process();


   StringVec_t fInputColName;
   TString fOutputColName;
   TString fMapFileName;
   Bool_t  fOutputIsTransparent;

   TArtRawEventObject *fRawData;
   TCatRawDataCategorized *fRawCategrized;
   TArtMapper *fMapper;
};
#endif // end of #ifdef TCATCATEGORIZINGPROCESSOR_H
