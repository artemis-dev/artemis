/* @file TEnergyCorrectionInMaterial.cc
 * @brief energy correction in material using range table 
 *
 * @date Create        : 2019-06-08 23:44:55 JST
 *       Last Modified : 2019-06-14 01:09:15 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#include "TEnergyCorrectionInMaterial.h"
#include "ICharge.h"
#include "ITrack.h"
#include "TRangeTableHelper.h"
#include "TDetectorGeometryInfo.h"
#include "TDataObject.h"
#include "TMath.h"
#include "TArtSystemOfUnit.h"
ClassImp(art::TEnergyCorrectionInMaterial)

using art::TEnergyCorrectionInMaterial;
using TArtSystemOfUnit::um;


TEnergyCorrectionInMaterial::TEnergyCorrectionInMaterial()
{

   fRangeTable.SetDoAuto(true);
   fInputCharge.SetDoAuto(true);
   fInputTrack.SetDoAuto(true);
   fOutputCharge.SetDoAuto(false);

   Register(fRangeTable("RangeTable","name of range table","rangeTable"));
   Register(fInputCharge("InputCharge","name of charge to be corrected","inputCharge"));
   Register(fOutputCharge("OutputCharge","name of corrected charge output","outputCharge"));
   Register(fInputTrack("InputTrack","name of optional track to be considered","inputTrack"));
   Register(fGeometry("Geometry","name of detector geometry information","geometry"));
   Register(fOverrideDeadLayer("OverrideDeadLayer","dead layer thickness to override detector information if positive (mm)",-1));
   
}

TEnergyCorrectionInMaterial::~TEnergyCorrectionInMaterial()
{
}

void TEnergyCorrectionInMaterial::Init(TEventCollection *col)
{
   if (!fInputCharge->GetClass()->InheritsFrom(TDataObject::Class())) {
      SetStateError(Form("%s does not inherits from %s",fInputCharge.Value().Data(),TDataObject::Class_Name()));
      return;
   }
   
   fOutputCharge.SetData(new TClonesArray(fInputCharge->GetClass()));
   col->Add(fOutputCharge.Value(),fOutputCharge.fData,fOutputIsTransparent);
}

void TEnergyCorrectionInMaterial::Process()
{
   fOutputCharge->Clear("C");
   if (!fInputTrack->GetEntriesFast()) return;
   const TVector3& trackDirection = dynamic_cast<ITrack*>(fInputTrack->UncheckedAt(0))->GetDirection();

   for (int i = 0, n = fInputCharge->GetEntriesFast(); i < n; ++i) {
      TDataObject *input = dynamic_cast<TDataObject*>(fInputCharge->UncheckedAt(i));
      Double_t charge = dynamic_cast<ICharge*>(input)->GetCharge();
      TDetectorGeometryInfo *info = static_cast<TDetectorGeometryInfo*>(fGeometry->UncheckedAt(input->GetID()));
      const TVector3& deadLayerDirection = info->GetDeadLayerDirection();
      const double deadLayer = fOverrideDeadLayer > 0 ? fOverrideDeadLayer : info->GetDeadLayer();
      const double thickness = deadLayer / TMath::Abs(deadLayerDirection.Dot(trackDirection)/(deadLayerDirection.Mag()*trackDirection.Mag()));

      const double defaultstep = 0.1 * um;
      const int nstep = TMath::CeilNint(thickness / defaultstep);
      const double step = thickness / nstep;
      for (int istep = 0; istep < nstep; ++istep) {
         charge += fRangeTable->R2DE(fRangeTable->E2R(charge)) * step;
      }

      TObject *obj = fOutputCharge->ConstructedAt(fOutputCharge->GetEntriesFast());
      input->Copy(*obj);
      dynamic_cast<ICharge*>(obj)->SetCharge(charge);
   }
}


   
