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

#include "TSRPPACPlaneSideDqdxProcessor.h"
#include "TMonotoneTableConverter.h"
#include "TMath.h"

using art::TSRPPACPlaneSideDqdxProcessor;

ClassImp(TSRPPACPlaneSideDqdxProcessor)

TSRPPACPlaneSideDqdxProcessor::TSRPPACPlaneSideDqdxProcessor()
{

   RegisterInputInfo("Converter1","name of dqdx converter1",fConverterName1,"srppac_xcl",&fConverter1,"art::TMonotoneTableConverter");
   RegisterInputInfo("Converter2","name of dqdx converter2",fConverterName2,"srppac_xcr",&fConverter2,"art::TMonotoneTableConverter");
   RegisterProcessorParameter("Combination","type of combination: 0 => ID0-IDl, 1 => ID1-IDr",fCombination,0);
}

TSRPPACPlaneSideDqdxProcessor::~TSRPPACPlaneSideDqdxProcessor()
{
}

void TSRPPACPlaneSideDqdxProcessor::Process()
{
   (fOutput)->Clear("C");
   if ((*fInput)->GetEntriesFast() < 2) return;


   double posw= 0.;
   double posl= 0.;
   double posr= 0.;
   double c0 = 0.;
   double cl = 0.;
   double cr = 0.;
   int id0 = 0;
   int idl = 0;
   int idr = 0;
 
//   printf("%p\n",*fConverter);
   const Int_t nData = (*fInput)->GetEntriesFast();
  
   Int_t id[nData];
   Double_t charge[nData];
   
   for (Int_t iData = 0; iData != nData; ++iData) {
      const TTimingChargeData *const inData = static_cast<TTimingChargeData*>((*fInput)->At(iData));
      id[iData] = inData->GetID();
      charge[iData] = inData->GetCharge();
   }

   id0 = id[0];
   c0 =  charge[0];

   for (int i=1; i != nData; ++i){
    if (id[i] == id[0]+1){
        idl = id[i]; 
        cl = charge[i];
        break;
      }
    }

   for (int j=1; j != nData; ++j){
    if (id[j] == id[0]-1){
        idr = id[j];
        cr = charge[j];
        break;
      }
    }


   if (fCombination == 0 && nData > 2) {
      if (TMath::IsNaN(c0) || TMath::IsNaN(cl) || TMath::IsNaN(cr) ) return;
      posw = (fTurned ? -1 : 1)  * ((id0 - fCenter + (0.5 - (*fConverter1)->Convert((c0 - cl)/(c0 + cl))) * ( 1.0 ) * 1.0) * fStripWidth  - fDetOffset)*((c0-cr) /(2*c0 - cl - cr)) + (fTurned ? -1 : 1)  * ((id0 - fCenter + (0.5 - (*fConverter2)->Convert((c0 - cr)/(c0 + cr))) * ( -1.0 ) * 1.0) * fStripWidth  - fDetOffset)*((c0-cl) / (2*c0 - cl- cr));
     } else {
         return;
      }

   if (fCombination == 0 && nData > 2) {
      if (TMath::IsNaN(c0) || TMath::IsNaN(cl) || TMath::IsNaN(cr) ) return;
      posl = (fTurned ? -1 : 1)  * ((id0 - fCenter + (0.5 - (*fConverter1)->Convert((c0 - cl)/(c0 + cl))) * ( 1.0 ) * 1.0) * fStripWidth  - fDetOffset);
     } 

   if (fCombination == 0 && nData > 2) {
      if (TMath::IsNaN(c0) || TMath::IsNaN(cl) || TMath::IsNaN(cr) ) return;
       posr = (fTurned ? -1 : 1)  * ((id0 - fCenter + (0.5 - (*fConverter2)->Convert((c0 - cr)/(c0 + cr))) * ( -1.0 ) * 1.0) * fStripWidth  - fDetOffset);
     } 

 
   TSRPPACPlaneData *outData = static_cast<TSRPPACPlaneData*>(fOutput->ConstructedAt(0));
   outData->SetID(id0);
   outData->SetCharge(c0);
   outData->SetPosition(posw);
   outData->SetProcessed(kTRUE);
 

}

