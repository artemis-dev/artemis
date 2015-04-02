/* $Id:$ */
/**
 * @file   TArtemisObject.cc
 * @date   Created : Dec 01, 2013 04:01:48 JST
 *   Last Modified : Jan 07, 2014 11:29:52 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TArtemisObject.h"

ClassImp(art::TArtemisObject);

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
