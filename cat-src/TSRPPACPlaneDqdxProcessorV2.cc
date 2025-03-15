/**
 * @file   TSRPPACPlaneDqdxProcessor.cc
 * @brief  sr-ppac plane processor with dqdx
 *
 * @date   Created       : 2019-08-30 19:38:25 JST
 *         Last Modified :
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shinsuke OTA
 */

#include "TSRPPACPlaneDqdxProcessorV2.h"
#include "TMonotoneTableConverter.h"
#include "TMath.h"

using art::TSRPPACPlaneDqdxProcessorV2;

ClassImp(TSRPPACPlaneDqdxProcessorV2)

TSRPPACPlaneDqdxProcessorV2::TSRPPACPlaneDqdxProcessorV2()
{
   RegisterInputInfo("Converter","name of dqdx converter",fConverterName,"srppac_xc0",
                     &fConverter, "art::TMonotoneTableConverter");
   RegisterProcessorParameter("Combination","type of combination: 0 => ID0-ID1, 1 => ID1-ID2",fCombination,0);
}

TSRPPACPlaneDqdxProcessorV2::~TSRPPACPlaneDqdxProcessorV2()
{
}

void TSRPPACPlaneDqdxProcessorV2::Process()
{
   fOutput->Clear("C");
   if((*fInput)->GetEntriesFast() < 2 ) return;
   
   double pos = 0.;
   double c0 = 0.;
   double c1 = 0.;
   double c2 = 0.;
   int id0 = 0;
   int id1 = 0;
   int id2 = 0;
   Double_t k = -1.;
   TTimingChargeData* inData0;   
   TTimingChargeData* inData1;   
   TTimingChargeData* inData2;   
//   printf("%p\n",*fConverter);
   if (fCombination == 0 && (*fInput)->GetEntriesFast() > 1) {
      inData0 = static_cast<TTimingChargeData*>((*fInput)->UncheckedAt(0));
      inData1 = static_cast<TTimingChargeData*>((*fInput)->UncheckedAt(1));
      id0 = inData0->GetID();
      id1 = inData1->GetID();
      c0 = inData0->GetCharge();
      c1 = inData1->GetCharge();
      if (TMath::IsNaN(c0) || TMath::IsNaN(c1)) return;
      pos = (fTurned ? -1 : 1)  * ((id0 - fCenter + (1 - (*fConverter)->Convert((c0 - c1)/(c0 + c1))) * ( id0 < id1 ? 1 : -1) * 0.5) * fStripWidth  - fDetOffset);
      k = (*fConverter)->Convert((c0 - c1)/(c0 + c1));
   } else if (fCombination == 1 && (*fInput)->GetEntriesFast() > 2) {
      inData0 = static_cast<TTimingChargeData*>((*fInput)->UncheckedAt(0));
      inData1 = static_cast<TTimingChargeData*>((*fInput)->UncheckedAt(1));
      inData2 = static_cast<TTimingChargeData*>((*fInput)->UncheckedAt(2));
      id0 = inData0->GetID();
      id1 = inData1->GetID();
      id2 = inData2->GetID();
      c1 = inData1->GetCharge();
      c2 = inData2->GetCharge();
      if (TMath::IsNaN(c1) || TMath::IsNaN(c2)) return;
//      pos = (*fConverter)->Convert(c1 - c2) ;
      pos = (fTurned ? -1 : 1)  * ((id0 - fCenter + ((*fConverter)->Convert(c1 - c2)) * ( id1 < id2 ? -1 : 1)  * 0.5 ) * fStripWidth  - fDetOffset);
      k =  ((*fConverter)->Convert(c1 - c2));
   } else {
      return;
   }
      
   TSRPPACPlaneDataV2 *outData = static_cast<TSRPPACPlaneDataV2*>(fOutput->ConstructedAt(0));
   outData->SetID(id0);
   outData->SetPosition(pos);
   outData->SetProcessed(kTRUE);
   outData->SetConvCoeff(k);
}

