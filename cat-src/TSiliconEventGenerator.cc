/* @file TSiliconEventGenerator.cc
 * @brief generate silicon event from trajectory
 *
 * @date Create        : 2019-05-21 20:36:50 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TSiliconEventGenerator.h"
#include "TObjArray.h"
#include "TCatTpcTrack.h"
#include "TTimingChargePositionData.h"
#include "TRangeTableHelper.h"
#include "TDetectorGeometryInfo.h"

ClassImp(art::TSiliconEventGenerator)

using art::TSiliconEventGenerator;

TSiliconEventGenerator::TSiliconEventGenerator()
{
   fInput.SetDoAuto(true);
   fOutput.SetDoAuto(true);
   fPosition.SetDoAuto(true);
   fRangeTable.SetDoAuto(true);
   
   Register(fInput("Input","name of input trajectory","track"));
   Register(fOutput("Output","name of output hit","hits"));
   Register(fPosition("PositionInfo","name of input information of position","sihit_true"));
   Register(fRangeTable("RangeTable","name of range table","rangeTableD"));
   Register(fThreshold("Threshold","threshold for charge",1.));
}

TSiliconEventGenerator::~TSiliconEventGenerator()
{
}

void TSiliconEventGenerator::Process()
{
   TClonesArray* const inputTrack = *fInput.fData;
   TClonesArray* const output = fOutput.fData;
   TObjArray* const position = *fPosition.fData;
   TRangeTableHelper* const rangeTable = *fRangeTable.fData;
   
   output->Clear("C");
   
   TCatTpcTrack *track = static_cast<TCatTpcTrack*>(inputTrack->UncheckedAt(0));
   const Int_t nSi = position->GetEntriesFast();
   Int_t siID = -1;
   for (Int_t i = 0; i < nSi; ++i) {
      TDetectorGeometryInfo *info = static_cast<TDetectorGeometryInfo*>(position->At(i));
      const Double_t siX = info->X();
      const Double_t endX =  track->GetEndPoint().X();
      const Double_t startX = track->GetStartPoint().X();
      const Double_t dirX = track->GetDirection().X();
      if (TMath::Abs(siX) > TMath::Abs(endX) ||  (siX -startX) * endX < 0) continue;

      const Double_t trackZAtSi = track->GetStartPoint().Z() + (siX - startX) / dirX * track->GetDirection().Z();
      if (TMath::Abs(trackZAtSi - info->Z()) < 0.684/2.) {
         siID = i;
         break;
      }
   }

   if (siID < 0) return;

   TDetectorGeometryInfo *info = static_cast<TDetectorGeometryInfo*>(position->At(siID));
   Double_t range = track->GetRange() / track->GetDirection().X() * (track->GetEndPoint().X() - info->X());
   Double_t charge = rangeTable->R2E(range);
//   Info("Process","orgrange = %g, si = %f, dirX = %f, endX = %f, range = %g, charge = %g, threshold = %f\n",track->GetRange(),info->X(),track->GetDirection().X(),track->GetEndPoint().X(),range,charge,fThreshold.fValue);
   if (charge < fThreshold.fValue) return;
   
   TTimingChargePositionData *data = static_cast<TTimingChargePositionData*> (output->ConstructedAt(output->GetEntriesFast()));
   data->SetCharge(charge);
   data->SetXYZ(info->X(),info->Y(),info->Z());
   
}
