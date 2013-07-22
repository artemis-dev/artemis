/* $Id:$ */
/**
 * @file   TRawDataV1190.cc
 * @date   Created : Jul 20, 2013 14:20:14 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRawDataV1190.h"

ClassImp(art::TRawDataV1190);

art::TRawDataV1190::TRawDataV1190()
   : fLeading(TRawDataObject::kInvalid), fTrailing(TRawDataObject::kInvalid)
{
}
art::TRawDataV1190::~TRawDataV1190()
{
}
