/* $Id:$ */
/**
 * @file   TArtFadcRawDataObject.cc
 * @date   Created : Dec 25, 2011 00:25:40 JST
 *   Last Modified : Dec 25, 2011 02:14:54 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtFadcRawDataObject.h"

ClassImp(TArtFadcRawDataObject);

TArtFadcRawDataObject::TArtFadcRawDataObject() 
   : TArtRawDataObject()
{
}

void TArtFadcRawDataObject::Clear(const Option_t*){
   nSample = 0;
  geo=-1; channel=-1; value=0x0;
  categoryid=-1; detectorid=-1; datatype=-1;
  return;
}
