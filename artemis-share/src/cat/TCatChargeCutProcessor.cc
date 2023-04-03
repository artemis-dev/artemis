/* $Id:$ */
/**
 * @file   TCatChargeCutProcessor.cc
 * @date   Created : Sep 29, 2012 22:29:46 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatChargeCutProcessor.h"

#include <TCatSimpleData.h>

TCatChargeCutProcessor::TCatChargeCutProcessor()
{
}
TCatChargeCutProcessor::~TCatChargeCutProcessor()
{
}

Bool_t TCatChargeCutProcessor::IsValid(const TCatSimpleData& data)
{
   // if name of output array is different from that of input array
//   printf("IsValid :\n");
   Double_t charge = data.GetCharge();
   Int_t id = data.GetDetID()-1;
//   printf("run id = %d\n",id);
   if (charge < fGateMin[id]) {
//      printf("gate min false\n");
      return kFALSE;
   }
   if (charge > fGateMax[id]) {
//      printf("gate max false\n");
      return kFALSE;
   }
//   printf("Valid\n");
   return kTRUE;
}
