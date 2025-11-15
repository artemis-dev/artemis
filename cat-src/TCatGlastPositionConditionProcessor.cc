/* @file TCatGlastPositionConditionProcessor.cc
 * @brief generate position information of glast silicon
 *
 * @date Create        : 2019-05-16 22:12:56 JST
 *       Last Modified : 2019-06-14 02:53:52 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCatGlastPositionConditionProcessor.h"

#include "TDetectorGeometryInfo.h"

using art::ribf113::TCatGlastPositionConditionProcessor;

ClassImp(art::ribf113::TCatGlastPositionConditionProcessor)


TCatGlastPositionConditionProcessor::TCatGlastPositionConditionProcessor()
{
   DoubleVec_t defaultOffset(3,0);
   Register(fGeometry("OutputName","name of output information","glastPos"));
   defaultOffset[0] = -128.5;
   defaultOffset[2] = 50.;
   Register(fOffsetLeft("OffsetLeft","offset of silicon",defaultOffset));
   defaultOffset[0] = 128.5;
   defaultOffset[2] = 50.;
   Register(fOffsetRight("OffsetRight","offset of silicon",defaultOffset));
   Register(fDeadLayer("DeadLayer","dead layer thickness [mm]",1.5));
}


TCatGlastPositionConditionProcessor::~TCatGlastPositionConditionProcessor()
{
}

void TCatGlastPositionConditionProcessor::Init(TEventCollection *col)
{
   fGeometry.SetData(new TClonesArray(TDetectorGeometryInfo::Class()));
   const DoubleVec_t &offsetLeft = fOffsetLeft;
   const DoubleVec_t &offsetRight = fOffsetRight;
   
   const Double_t stripWidth = 0.684; // mm
   
   for (Int_t i = 0, n = 128; i < n; ++i) {
      Double_t z = stripWidth * (63 - i + 0.5);
      TDetectorGeometryInfo *info1 = static_cast<TDetectorGeometryInfo*>(fGeometry->ConstructedAt(127-i));
      TDetectorGeometryInfo *info2 = static_cast<TDetectorGeometryInfo*>(fGeometry->ConstructedAt(i+128));
      info1->SetX(offsetLeft[0]);
      info1->SetY(offsetLeft[1]);
      info1->SetZ(z + offsetLeft[2]);
      info1->SetID(127-i);
      info2->SetX(offsetRight[0]);
      info2->SetY(offsetRight[1]);
      info2->SetZ(z + offsetRight[2]);
      info2->SetID(i+128);
      info1->SetDeadLayer(fDeadLayer);
      info1->SetDeadLayerDirecion(-1.,0.,0.);
      info2->SetDeadLayer(fDeadLayer);
      info2->SetDeadLayerDirecion(1.,0.,0.);
      
   }
   fGeometry->SetName(fGeometry.Value());
   col->AddInfo(fGeometry.Value(),fGeometry.fData,fOutputIsTransparent);
   Info("Init","Geometry info for glast is added : %s",fGeometry.Value().Data());
}
