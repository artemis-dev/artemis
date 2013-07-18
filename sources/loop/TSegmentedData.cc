/* $Id:$ */
/**
 * @file   TSegmentedData.cc
 * @date   Created : Jul 16, 2013 22:16:01 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSegmentedData.h"

ClassImp(art::TSegmentedData);

art::TSegmentedData::TSegmentedData()
   : TClonesArray("TObjArray") {
}
art::TSegmentedData::~TSegmentedData()
{
}
