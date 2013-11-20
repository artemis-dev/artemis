/* $Id:$ */
/**
 * @file   TEventHeader.cc
 * @date   Created : Nov 20, 2013 13:20:58 JST
 *   Last Modified : Nov 20, 2013 15:23:56 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TEventHeader.h"


using art::TEventHeader;

ClassImp(TEventHeader);

TEventHeader::TEventHeader()
   : fRunName(), fRunNumber(0), fTimestamp(0)
{
}
TEventHeader::~TEventHeader()
{
}
