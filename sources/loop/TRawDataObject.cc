/* $Id:$ */
/**
 * @file   TRawDataObject.cc
 * @date   Created : Jul 16, 2013 17:16:29 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRawDataObject.h"

ClassImp(art::TRawDataObject);

using art::TRawDataObject;

const Int_t TRawDataObject::kInvalid = -1E+8;

TRawDataObject::TRawDataObject()
   : fSegID(kInvalid), fGeo(kInvalid), fCh(kInvalid),
     fCatID(kInvalid),fDetID(kInvalid),fType(kInvalid)
{
}

TRawDataObject::~TRawDataObject()
{
}
