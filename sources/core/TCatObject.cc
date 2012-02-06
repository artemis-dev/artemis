/* $Id:$ */
/**
 * @file   TCatObject.cc
 * @date   Created : Jan 31, 2012 11:31:50 JST
 *   Last Modified : Feb 04, 2012 22:55:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatObject.h"

ClassImp(TCatObject);

TCatObject::TCatObject()
   : fCategoryID(-1), fDetectorID(-1), fOptLevel(0) {
}
TCatObject::~TCatObject()
{
}
