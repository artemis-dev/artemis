/* $Id:$ */
/**
 * @file   TRawDataTriggeredList.cc
 * @author Rin Yokoyama <yokoyama@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#include "TRawDataTriggeredList.h"

ClassImp(art::TRawDataTriggeredList);

art::TRawDataTriggeredList::TRawDataTriggeredList()
   : fADC(0),fTSI_Hi(0),fTSI_Lo(0),fTSI(0),fEventCount(0)
{
}
art::TRawDataTriggeredList::~TRawDataTriggeredList()
{
}
