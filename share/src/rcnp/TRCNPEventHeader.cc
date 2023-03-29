/* $Id:$ */
/**
 * @file   TRCNPEventHeader.cc
 * @date   Created : Nov 20, 2013 13:20:58 JST
 *   Last Modified : Nov 22, 2013 18:40:04 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRCNPEventHeader.h"


using art::TRCNPEventHeader;

ClassImp(TRCNPEventHeader);

TRCNPEventHeader::TRCNPEventHeader()
   : fRunName(), fRunNumber(0), fTimestamp(0), fEventNumber(0), fEventNumberTotal(0), fBlockNumber(0),fInputRegister(0)
{
}
TRCNPEventHeader::~TRCNPEventHeader()
{
}
