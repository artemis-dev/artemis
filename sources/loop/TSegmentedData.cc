/* $Id:$ */
/**
 * @file   TSegmentedData.cc
 * @date   Created : Jul 16, 2013 22:16:01 JST
 *   Last Modified : Nov 21, 2013 11:34:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSegmentedData.h"

ClassImp(art::TSegmentedData);

TClonesArray *art::TSegmentedData::fgArray = NULL;

art::TSegmentedData::TSegmentedData()
{
   if (!fgArray) fgArray = new TClonesArray("TObjArray");
}
art::TSegmentedData::~TSegmentedData()
{
}
