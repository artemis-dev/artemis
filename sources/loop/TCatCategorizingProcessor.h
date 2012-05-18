/* $Id:$ */
/**
 * @file   TCatCategorizingProcessor.h
 * @date   Created : May 13, 2012 18:13:49 JST
 *   Last Modified : May 18, 2012 14:08:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATCATEGORIZINGPROCESSOR_H
#define TCATCATEGORIZINGPROCESSOR_H

#include <TCatProcessor.h>
#include <TCatEventCollection.h>

#include <TArtEventObject.h>
#include <TArtRawDataCategorized.h>

class TCatCategorizingProcessor  : public TCatProcessor {

public:
   TCatCategorizingProcessor();
   ~TCatCategorizingProcessor();

   virtual void Init(TCatEventCollection *col);
   virtual void Process();

   TArtRawEventObject *fRawData;
   TArtRawDataCategorized *fRawCategrized;
};
#endif // end of #ifdef TCATCATEGORIZINGPROCESSOR_H
