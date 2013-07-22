/* $Id:$ */
/**
 * @file   TRawDataFadc.cc
 * @date   Created : Jul 21, 2013 09:21:31 JST
 *   Last Modified : Jul 22, 2013 08:59:44 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRawDataFadc.h"

ClassImp(art::TRawDataFadc);

art::TRawDataFadc::TRawDataFadc()
   : fNumSample(0),fTimestamp(TRawDataObject::kInvalid),fOffset(TRawDataObject::kInvalid),fPattern(0)
{
}
art::TRawDataFadc::~TRawDataFadc()
{
}
