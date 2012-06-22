/* $Id:$ */
/**
 * @file   TCatRawDataV1190.cc
 * @date   Created : Jun 22, 2012 19:22:47 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatRawDataV1190.h"

TCatRawDataV1190::TCatRawDataV1190()
{
}

TCatRawDataV1190::TCatRawDataV1190(Int_t igeo, Int_t ich)
: fReference(-1E10),fLeading(-1E10),fTrailing(-1E10),fWidth(-1E10)
{
   SetGeo(igeo);
   SetCh(ich);
}

TCatRawDataV1190::~TCatRawDataV1190()
{
}
