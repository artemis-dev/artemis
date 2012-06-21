/* $Id:$ */
/**
 * @file   TCatEventStore.cc
 * @date   Created : May 04, 2012 00:04:01 JST
 *   Last Modified : May 16, 2012 15:26:56 JST
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatEventStore.h"

TCatEventStore::TCatEventStore()
{
   fObjects = new TList;
}
TCatEventStore::~TCatEventStore()
{
   if (fObjects) delete fObjects;
}





