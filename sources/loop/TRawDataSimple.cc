/* $Id:$ */
/**
 * @file   TRawDataSimple.cc
 * @date   Created : Jul 22, 2013 18:22:37 JST
 *   Last Modified : Jul 22, 2013 18:17:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRawDataSimple.h"

ClassImp(art::TRawDataSimple);

art::TRawDataSimple::TRawDataSimple()
   : fValue(TRawDataObject::kInvalid)
{
}
art::TRawDataSimple::~TRawDataSimple()
{
}
