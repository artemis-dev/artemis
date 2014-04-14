/* $Id:$ */
/**
 * @file   TEventHeader.cc
 * @date   Created : Nov 20, 2013 13:20:58 JST
 *   Last Modified : Nov 22, 2013 18:40:04 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TEventHeader.h"


using art::TEventHeader;

ClassImp(TEventHeader);

TEventHeader::TEventHeader()
   : fRunName(), fRunNumber(0), fTimestamp(0), fEventNumber(0), fEventNumberTotal(0)
{
}
TEventHeader::~TEventHeader()
{
}
