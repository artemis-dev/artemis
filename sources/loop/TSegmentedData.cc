/* $Id:$ */
/**
 * @file   TSegmentedData.cc
 * @date   Created : Jul 16, 2013 22:16:01 JST
 *   Last Modified : Nov 25, 2013 11:55:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSegmentedData.h"

ClassImp(art::TSegmentedData);



art::TSegmentedData::TSegmentedData()
   : fArray(NULL)
{
   if (!fArray) fArray = new TClonesArray("TObjArray");
}
art::TSegmentedData::~TSegmentedData()
{
   delete fArray;
}
