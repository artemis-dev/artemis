/* $Id:$ */
/**
 * @file   TArtemisObject.cc
 * @date   Created : Dec 01, 2013 04:01:48 JST
 *   Last Modified : 2016-08-19 10:33:31 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TArtemisObject.h"

ClassImp(art::TArtemisObject)

using art::TArtemisObject;


TArtemisObject::TArtemisObject()
{
}

TArtemisObject::TArtemisObject(const TArtemisObject &obj)
   : TObject(obj)
{
}


TArtemisObject::~TArtemisObject()
{
}
