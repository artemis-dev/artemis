/* @file TTimingChargePositionMappingProcessor.cc
 * @brief timing charge and position mapping 
 *
 * @date Create        : 2019-05-15 15:56:25 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TTimingChargePositionMappingProcessor.h"

#include "TTimingChargePositionData.h"
#include "TClonesArray.h"
#include "TDetectorGeometryInfo.h"

ClassImp(art::TTimingChargePositionMappingProcessor)

using art::TTimingChargePositionMappingProcessor;

TTimingChargePositionMappingProcessor::TTimingChargePositionMappingProcessor()
   : TTimingChargeMappingProcessor(), fPosition(NULL)
{
   OverrideOutputCollection("OutputCollection","name of output collection",
                            fOutputColName,TString("output"),
                            &fPlastic, TClonesArray::Class_Name(), TTimingChargePositionData::Class_Name());

   RegisterInputInfo("PositionInfo","information of position",
                              fPositionInfoName,TString("glastPos"),
                              &fPosition,TObjArray::Class_Name());
   
}

TTimingChargePositionMappingProcessor::~TTimingChargePositionMappingProcessor()
{
}

void TTimingChargePositionMappingProcessor::Process()
{
   TTimingChargeMappingProcessor::Process();
//   printf("fPosition = %p #hits = %d\n",fPosition,fPlastic->GetEntriesFast());
   
   for (Int_t i = 0, n = (fPlastic->GetEntriesFast()); i < n; ++i) {
      TTimingChargePositionData *pos = static_cast<TTimingChargePositionData*> (fPlastic->UncheckedAt(i));
      const int id = pos->GetID();
      TDetectorGeometryInfo *posInfo = static_cast<TDetectorGeometryInfo*>((*fPosition)->At(id));
//      printf("posInfo %p\n",posInfo);

      posInfo->IPosition::Copy(*pos);
   }
}





