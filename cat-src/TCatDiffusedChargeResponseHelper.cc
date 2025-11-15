/**
 * @file   TCatDiffusedChargeResponseHelper.cc
 * @brief  helper to calculate the pad response for the diffused charge
 *
 * @date   Created       : 2016-09-12 11:11:09 JST
 *         Last Modified : 2020-09-03 19:24:30 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatDiffusedChargeResponseHelper.h"
#include <TMath.h>
#include <TCatReadoutPadArray.h>
#include <TNArray.h>
#include <TArtSystemOfUnit.h>
#include <TGraph.h>
#include <TCatReadoutPadConditionProcessor.h>
#include <TCatReadoutPad.h>
#include <TFile.h>
#include <TDirectory.h>
#include "TRangeTableHelper.h"
#include <TRandomGraph.h>
#include <TH2Poly.h>
#include <TStopwatch.h>
#include "TArtMathUtil.h"

//#define DEBUG

#ifdef DEBUG
#include <TH2F.h>
#include <TEllipse.h>
#include <TLine.h>
#endif

using TArtSystemOfUnit::deg;
using TArtSystemOfUnit::eV;
using art::TCatDiffusedChargeResponseHelper;
using namespace art::TArtMathUtil;

ClassImp(TCatDiffusedChargeResponseHelper)

#ifdef DEBUG
namespace {
   TH2F *hElectrons; // electron position
   std::vector<TLine> sides(3); // side lines of pad
   TLine track; // track
   TEllipse stepStart; // calculation step start
   TEllipse stepEnd; // end point of step
};
#endif

TCatDiffusedChargeResponseHelper::TCatDiffusedChargeResponseHelper()
   : fDisplacementTableX(NULL), fDisplacementTableZ(NULL), fAdditionalRange(0.)
{

}

TCatDiffusedChargeResponseHelper::~TCatDiffusedChargeResponseHelper()
{
}

TCatDiffusedChargeResponseHelper::TCatDiffusedChargeResponseHelper(const TCatDiffusedChargeResponseHelper& rhs)
{
}

TCatDiffusedChargeResponseHelper& TCatDiffusedChargeResponseHelper::operator=(const TCatDiffusedChargeResponseHelper& rhs)
{
   if (this != &rhs) {
      
   }
   return *this;
}

void TCatDiffusedChargeResponseHelper::Init(TEventCollection *col)
{
   const Int_t nPad = (fReadoutPadArray)->GetEntriesFast();
   fPadCenter.resize(nPad);
   fPadVertex.resize(nPad);
   for (Int_t i = 0; i < nPad; ++i) {
      TCatReadoutPad *pad = (TCatReadoutPad*)(fReadoutPadArray)->UncheckedAt(i);
      fPadCenter[i].Set(pad->X(),pad->Y());
      fPadVertex[i].Set(pad->VertexX()[0], pad->VertexY()[0]);
   }
   fR2DE = (fRangeTable)->GetR2DE();
   fRandomBrag = new TRandomGraph(fR2DE);

   if (!fDisplacementTableXName.IsNull()) {
      fDisplacementTableX = reinterpret_cast<TNArray**>(col->GetInfoRef(fDisplacementTableXName));
      if (!fDisplacementTableX) {
         fParent->SetStateError(TString::Format("No such TNArray '%s'",fDisplacementTableXName.Data()));
         return;
      }
   }
   if (!fDisplacementTableZName.IsNull()) {
      fDisplacementTableZ = reinterpret_cast<TNArray**>(col->GetInfoRef(fDisplacementTableZName));
      if (!fDisplacementTableZ) {
         fParent->SetStateError(TString::Format("No such TNArray '%s'",fDisplacementTableZName.Data()));
         return;
      }
   }


   fTpcGasProp->Print();
   
#ifdef DEBUG
   hElectrons = new TH2F("hElectrons","electron position",100,-50.,50.,100,-50.,50.);
#endif   
}


void TCatDiffusedChargeResponseHelper::Copy(TCatDiffusedChargeResponseHelper &obj) const
{
   obj.fRandomBrag = fRandomBrag;
   obj.fR2DE = fR2DE;
   obj.fReadoutPadArray = fReadoutPadArray;
   const Int_t nPad = (fReadoutPadArray)->GetEntriesFast();
   obj.fPadCenter.resize(nPad);
   obj.fPadVertex.resize(nPad);
   for (Int_t i=0; i<nPad; i++) {
      TCatReadoutPad *pad = (TCatReadoutPad*)(fReadoutPadArray)->UncheckedAt(i);
      obj.fPadCenter[i].Set(pad->X(),pad->Y());
      obj.fPadVertex[i].Set(pad->VertexX()[0], pad->VertexY()[0]);
   }
   obj.fDefaultIntegrateStep = fDefaultIntegrateStep;
   obj.fReadoutPlaneOffset = fReadoutPlaneOffset;
   obj.fResponseTable = fResponseTable;
   obj.fRangeTable = fRangeTable;
   obj.fDisplacementTableX = fDisplacementTableX;
   obj.fDisplacementTableZ = fDisplacementTableZ;
   obj.fDisplacementTableXName = fDisplacementTableXName;
   obj.fDisplacementTableZName = fDisplacementTableZName;
   obj.fActiveArea = fActiveArea;
   obj.fUseDisplacement3D = fUseDisplacement3D;
   obj.fAdditionalRange = fAdditionalRange;
   obj.fTpcGasProp = fTpcGasProp;
   
}

void TCatDiffusedChargeResponseHelper::Register(TProcessor *proc)
{
   proc->Register(fDefaultIntegrateStep("DefaultIntegrateStep","default integrate step (mm)",1.));
   proc->Register(fReadoutPlaneOffset("ReadoutPlaneOffset","offset of readout plane relative to the beam axis",-50.));
   proc->Register(fReadoutPadArray("ReadoutPad","array of readout pad","readoutPad"));
   proc->Register(fResponseTable("ResponseTableName","name of the response table","responseTable"));
   proc->Register(fActiveArea("ActiveArea","active area as box (x1,y1,z1,x2,y2,z2)",DoubleVec_t(6,0)));
   proc->Register(fRangeTable("RangeTable","name of range table",TString("rangeTable")));
   proc->RegisterInputInfo("DisplacementTableXName","name of position displacement table for X",fDisplacementTableXName,
                           TString(""));
   proc->RegisterInputInfo("DisplacementTableZName","name of position displacement table for Z",fDisplacementTableZName,
                           TString(""));
   proc->Register(fUseDisplacement3D("UseDisplacement3D","use 3d-position to calculate displacement",0));
   proc->Register(fTpcGasProp("TpcGasProp","name of tpc property collection","tpcGasProperty"));
}

void TCatDiffusedChargeResponseHelper::SetTrack(const TVector3& start, const TVector3& end)
{
//   if (TMath::IsNaN(start.Mag()) || TMath::IsNaN(end.Mag())) {
//      return;
//   }
//   start.Print();
//   end.Print();
   
   fTrackStart = start;
   fTrackEnd = end;
   fDirection = (fTrackEnd - fTrackStart);
   fDirection *= (fAdditionalRange + fDirection.Mag()) / fDirection.Mag();
   fTrackEnd = start + fDirection;
   fUnitDirection = fDirection.Unit();
   fRange = fDirection.Mag();
   fNumStep = TMath::CeilNint(fRange / fDefaultIntegrateStep) * 2; // for sure
   fStep = fRange / fNumStep;
   EstimateActiveRange();
#if 0
   Info("SetTrack","Calculate charge for ");
   Info("SetTrack","Start : %10.5f, %10.5f, %10.5f",start.X(),start.Y(),start.Z());
   Info("SetTrack","End   : %10.5f, %10.5f, %10.5f",end.X(),end.Y(),end.Z());
   Info("SetTrack","Energy: %10.5f",(fRangeTable)->GetR2E()->Eval(fRange));
   Info("SetTrack","dE/dX : %10.5f",(fRangeTable)->GetR2DE()->Eval(fRange));
#endif
}


//////////////////////////////////////////////////////////////////////
/// Get response function of a certain pad.
///
///

void TCatDiffusedChargeResponseHelper::GetResponse(Int_t id, Double_t &charge, Double_t &closest, Double_t &angle_closest, Double_t &rangeToEnd, Double_t &trackLength) const
{
   // initialization
   charge = 0;
   rangeToEnd = 0;
   trackLength = 0;
   closest = 0;

   // center of pad
   const TVector2 &c2 = fPadCenter[id];
   // one vertex of pad
   const TVector2 &v1 = fPadVertex[id];
   Int_t iStepStart = 0;
   Int_t iStepEnd = fNumStep;
   
   // find the range of step
   {
      const Double_t x1 = fTrackStart.X();
      const Double_t y1 = fTrackStart.Z();
      const Double_t x2 = fTrackEnd.X();
      const Double_t y2 = fTrackEnd.Z();
      const Double_t vx = x2 - x1;
      const Double_t vy = y2 - y1;
      const Double_t dx = c2.X() - x1;
      const Double_t dy = c2.Y() - y1;
      const Double_t cx1 = c2.X();
      const Double_t cy1 = c2.Y();
      const Double_t vmag2 = vx * vx + vy * vy;
      const Double_t vmag = TMath::Sqrt(vmag2);
      // trajectory lengh from start point to the closest point.
      const Double_t len = (vx * dx + vy * dy)/vmag;
      const Double_t step2D = fStep * TMath::Sqrt(fUnitDirection.X()*fUnitDirection.X()+fUnitDirection.Z()*fUnitDirection.Z());
      const Int_t iStepMean = TMath::FloorNint(len/step2D);
      // calculate over the mean +- 15mm (should depend on the pad size)
      iStepStart = std::max(TMath::FloorNint(iStepMean - 20/step2D), 0);
      iStepEnd   = std::min(TMath::CeilNint(iStepMean + 20/step2D), fNumStep);
      
#if 0
      printf("x1 = %f, y1 = %f, x2 = %f, y2 = %f, len = %f\n",x1,y1,x2,y2,len);
      printf("c2.X() = %f, c2.Y() = %f\n",c2.X(),c2.Y());
      printf("vx = %f, dx = %f, vy = %f, dy = %f\n",vx,dx,vy,dy);
      printf("iStepMean = %d, step2D = %f, fNumStep = %d\n",iStepMean,step2D,fNumStep);
      printf("iStart = %d, iEnd = %d\n",iStepStart,iStepEnd);
#endif      
      if (iStepStart >= iStepEnd) {
//         closest = TMath::Sqrt(dx*dx + dy*dy);
         charge = TMath::Limits<Double_t>::Epsilon();;
         trackLength = TMath::Limits<Double_t>::Epsilon();
         closest = TMath::Abs(( (x1 - cx1) * vy - (y1 - cy1) * vx ) / vmag);
         if (TMath::IsNaN(closest)) {
            closest = 1000.;
         }
         return;
      }
//      closest = ( (x1 - cx1) * vy - (y1 - cy1) * vx ) / vmag;
      Double_t closestX;
      Double_t closestY;
      art::TArtMathUtil::GetClosestPoint2D(x1,y1,x2,y2,cx1,cy1,
                                           closestX, closestY);
//      printf("cx1 = %f, cy1 = %f, closestX = %f, closestY = %f\n",cx1,cy1,closestX,closestY);
//      printf("closest = %f, length = %f\n",closest, TMath::Sqrt((closestX - cx1) * (closestX - cx1) + (closestY - cy1) * ( closestY - cy1)));
      const Double_t phi = TMath::ATan2(-(closestY - cy1), -(closestX - cx1))
         - TMath::ATan2( ( c2.Y() - v1.Y() ), (c2.X() - v1.X()));
      angle_closest = TVector2::Phi_0_2pi(phi)/deg;
      angle_closest -= TMath::Floor(angle_closest/120) * 120;
      if (angle_closest > 60.) angle_closest = TMath::Abs(120 - angle_closest);

      rangeToEnd = ((x2 - closestX) * vx + (y2 - closestY) * vy) / vmag;
   }

#ifdef DEBUG
   TCanvas *canvas = new TCanvas("c1","c1",800,800);
   track = TLine(fTrackStart.X(),fTrackStart.Z(),fTrackEnd.X(),fTrackEnd.Z());
   stepStart.SetX1(fTrackStart.X() + iStepStart * fStep * fUnitDirection.X());
   stepStart.SetY1(fTrackStart.Z() + iStepStart * fStep * fUnitDirection.Z());
   stepStart.SetR2(2);
   stepEnd.SetX1(fTrackStart.X() + iStepEnd * fStep * fUnitDirection.X());
   stepEnd.SetY1(fTrackStart.Z() + iStepEnd * fStep * fUnitDirection.Z());
   stepEnd.SetR2(2);
   TCatReadoutPad *pad = (TCatReadoutPad*)(*fReadoutPadArray)->UncheckedAt(id);
   for (Int_t i = 0; i < 3; ++i) {
      sides[i] = TLine(pad->VertexX()[i],pad->VertexY()[i],
                       pad->VertexX()[(i+1)%3],pad->VertexY()[(i+1)%3]);
   }

   // draw
   hElectrons->Draw();
   for (Int_t i = 0; i < 3; ++i) {
      sides[i].Draw();
   }
   track.Draw();
   stepStart.Draw();
   stepEnd.Draw();
   return;
#endif


   
   // integrate over the found integration range

   // reset closest distance
   closest = TMath::Limits<Double_t>::Max();
   for (Int_t i = iStepStart; i < iStepEnd; ++i) {
      Double_t angle, drift,distance;
      Double_t rmidX = fTrackStart.X() + (i+0.5)*fStep * fUnitDirection.X();
      const Double_t rmidY = fTrackStart.Y() + (i+0.5)*fStep * fUnitDirection.Y();
      Double_t rmidZ = fTrackStart.Z() + (i+0.5)*fStep * fUnitDirection.Z();
      drift = rmidY  - fReadoutPlaneOffset;
      Double_t dx = TMath::QuietNaN();
      Double_t dz = TMath::QuietNaN();
      if (fDisplacementTableX) {
         if (fUseDisplacement3D) {
            Double_t pd_args[3] = {rmidX, drift, rmidZ};
            dx = (*fDisplacementTableX)->Eval(pd_args);
         } else {
            Double_t pd_args[2] = {rmidX, drift};
            dx = (*fDisplacementTableX)->Eval(pd_args);
         }
         if (TMath::IsNaN(dx)) {
            
         } else {
            rmidX += dx;
         }
         

      }
      if (fDisplacementTableZ) {
         if (fUseDisplacement3D) {
            Double_t pd_args[3] = {rmidX, drift, rmidZ};
            dz = (*fDisplacementTableZ)->Eval(pd_args);
         } else {
            Double_t pd_args[2] = {TMath::Abs(rmidZ), drift};
            Double_t sign = (rmidZ>0) - (rmidZ<0);
            dz = sign * (*fDisplacementTableZ)->Eval(pd_args);
         }
         if (TMath::IsNaN(dz)) {
            
         } else {
            rmidZ += dz;
         }
      }
       
      

      const Double_t c2bmidX = rmidX - c2.X();
      const Double_t c2bmidY = rmidZ - c2.Y();
      distance = TMath::Sqrt(c2bmidX * c2bmidX + c2bmidY * c2bmidY);
      const Double_t phi = TMath::ATan2(-c2bmidY,-c2bmidX) 
         - TMath::ATan2( ( c2.Y() - v1.Y() ), (c2.X() - v1.X()));
      angle = TVector2::Phi_0_2pi(phi)/deg;
      angle -= TMath::Floor(angle/120) * 120 ;

      if (drift < 0) drift = 0;
      if (angle > 60.) angle = TMath::Abs(120 - angle);
      const Double_t diffusion = fTpcGasProp->GetTransDiff(drift);
      
      Double_t x[] = {angle, distance, diffusion };
#if 1
      if (distance < closest) {
         closest = distance;
         angle_closest = angle;
         rangeToEnd   = fRange - (i+0.5)*fStep;
      }
#endif      
      if (TMath::Abs(rmidX) > 52 || TMath::Abs(rmidZ) > 52) continue;

      Double_t weight =(fResponseTable)->Eval(x) * fTpcGasProp->GetAliveRatio(drift);
      if (TMath::IsNaN(weight)) {
         weight = 0.;
      } else {
         Double_t dedx = 0;
         if (fRange - (i) * fStep > 0) {
            dedx += (fRangeTable)->R2DE(fRange - (i)*fStep) * fStep;
            if (fRange - (i+1) * fStep > 0) {
               dedx += (fRangeTable)->R2DE(fRange - (i+1)*fStep) * fStep;
               dedx *= 0.5;
            }
         }
         if (weight < 0 ) weight = 0.;
         
         // charge calculation
         charge += dedx * weight;
         trackLength += fStep * weight;
         
//         if (weight > 0.001) {
//            trackLength += fStep;
//
//         }
         
         
      }
#if 0
      if (fTrackEnd.Y() - fTrackStart.Y() > 200.) {
      if (iStepStart == i) {
         fTrackStart.Print();
         fTrackEnd.Print();
         fUnitDirection.Print();
         printf("[%03d] Start d = %10f, (%10f,%10f,%10f), (%10f,%10f)\n",id, distance,rmidX,rmidY,rmidZ,c2.X(),c2.Y());
      }
      if (iStepEnd - 1 == i) {
         printf("[%03d] End   d = %10f, (%10f,%10f,%10f), (%10f,%10f) c = %f\n",id, distance,rmidX,rmidY,rmidZ,c2.X(),c2.Y(),charge);
      }
      }
#endif      
   }
   
#if 0

   printf("id = %d charge = %f\n",id,charge);
   printf("**********************************************************************\n");
   
#endif
}


// Estimate distance to end of effective track
void TCatDiffusedChargeResponseHelper::EstimateActiveRange()
{
   const Int_t kDimension = 3;
   const Int_t kPlanes = 6;
   const TVector3& start = fTrackStart;
   const TVector3& end = fTrackEnd;
   const TVector3& unit = fUnitDirection;
   Double_t &longer = fLongerRange;
   Double_t &shorter = fShorterRange;
   
   Bool_t startIsInside = kTRUE;
   Bool_t endIsInside = kTRUE;
   const DoubleVec_t& activeArea = fActiveArea.Value();
   for (Int_t iDim = 0; iDim < kDimension; ++iDim) {
      if (startIsInside && !IsWithinRange(start[iDim],activeArea[iDim],activeArea[iDim+kDimension])) {
         startIsInside = kFALSE;
      }
      if (endIsInside && !IsWithinRange(end[iDim],activeArea[iDim],activeArea[iDim+kDimension])) {
         endIsInside = kFALSE;
      }
   }
   if (fParent && fParent->GetVerboseLevel() > 0) {
      Info("EstimateActiveRange","startIsInside = %d, endIsInside = %d",startIsInside,endIsInside);
   }
   // default assuming start and end are inside active area
   shorter = 0;
   longer = fRange;
   
   if (!endIsInside || !startIsInside) {
      std::vector<TVector3> vertex;
      for (Int_t iPlane = 0; iPlane < kPlanes; ++iPlane) {
         Int_t axis0 = (iPlane % kDimension);
         Int_t axis1 = ((iPlane + 1) % kDimension);
         Int_t axis2 = ((iPlane + 2) % kDimension);
         if ((activeArea[iPlane]-start[axis0])/unit[axis0] < 0) continue;
         TVector3 point = start + (activeArea[iPlane] - start[axis0])/unit[axis0] * unit;
         if ((point - start).Mag() > fRange) continue;
         if (IsWithinRange(point[axis1],activeArea[axis1],activeArea[axis1 + kDimension]) &&
             IsWithinRange(point[axis2],activeArea[axis2],activeArea[axis2 + kDimension])) {
            // vertex at this plane is found
            vertex.push_back(point);
         }
      }
      if (startIsInside && vertex.size() == 1) {
         shorter = (vertex[0] - end).Mag();
      } else if (endIsInside && vertex.size() == 1) {
         longer = (vertex[0]-end).Mag();
      } else if (vertex.size() == 2) {
         shorter = (vertex[0] - end).Mag();
         longer = (vertex[1] - end).Mag();
         if (shorter > longer) {
            Double_t tmp = longer;
            longer = shorter;
            shorter = tmp;
         }
      }
   }
   if (fParent && fParent->GetVerboseLevel() > 0) {
      Info("EstimateActiveRange","shorter = %f, longer = %f (from end)",shorter,longer);
   }
}


Bool_t TCatDiffusedChargeResponseHelper::GeneratedElectronAt(TVector3 &point)
{
   Double_t mag = fRange - fRandomBrag->Generate(fShorterRange,fLongerRange);
   point = fTrackStart + mag * fUnitDirection;
   Double_t y = point.Y();
   const DoubleVec_t& activeArea = fActiveArea.Value();
   if (!IsWithinRange(y,activeArea[1],activeArea[4])) return kFALSE;
   Double_t drift = (y-fReadoutPlaneOffset);
   Double_t sigma = fTpcGasProp->GetTransDiff(drift);
   Double_t x = point.X();
   Double_t z = point.Z();
   Double_t dx = 0.;
   Double_t dz = 0.;
   if (fDisplacementTableX) {
      if (fUseDisplacement3D) {
         Double_t val[3] = {x,drift,z};
         dx = (*fDisplacementTableX)->Eval(val);
      } else {
         Double_t val[2] = {x,drift};
         dx = (*fDisplacementTableX)->Eval(val);
      }
   }
#if 0   
   if (x > 0) {
      printf("x = %f, range = %f, shorter = %f, longer = %f, mag = %f\n",x,fRange,fShorterRange,fLongerRange,mag);
   }
#endif   
   if (fDisplacementTableZ) {
      if (fUseDisplacement3D) {
         Double_t val[3] = {x, drift, z};
         dz = (*fDisplacementTableZ)->Eval(val);
      } else {
         Double_t val[2] = {TMath::Abs(z),drift};
         Double_t sign = (z>0) - (z<0);
         dz = sign * (*fDisplacementTableZ)->Eval(val);
      }
   }
   x = gRandom->Gaus(x + dx,sigma);
   z = gRandom->Gaus(z + dz,sigma);
   point.SetXYZ(x,y,z);
   return kTRUE;
}


// modify track according to the displacement at start and end
void TCatDiffusedChargeResponseHelper::DisplaceTrack(TVector3& start, TVector3& end)
{
   
   const Int_t kNumDim = 3;
   const Int_t kNumPl = 6;
   const TVector3 orgStart(start);
   const TVector3 orgEnd(end);
   const TVector3 orgDir(end - start);
   const TVector3 orgUnit(orgDir.Unit());

   for (int iDim = 0; iDim < kNumDim; ++iDim) {
      
   }

   
}



#if 0
// static function to test the class
void TCatDiffusedChargeResponseHelper::Test(Int_t id, const TVector3& start, const TVector3& end, Double_t &charge, Double_t &mccharge)
{
   // get pad array
   TCatReadoutPadArray *array = NULL;
   TCatReadoutPadConditionProcessor *proc = new TCatReadoutPadConditionProcessor;
   TEventCollection *col = new TEventCollection;
   Double_t wf = 37*eV; // eV
   proc->SetPadType(2);
   proc->Init(col);
   array = proc->GetPadArray();


   TRangeTableHelper *rangeTable = new TRangeTableHelper;
   rangeTable->SetFileName("data/DeuteronDeuterium/DeuteronDeuterium064.txt.out");
   rangeTable->SetTKECutoff(10.*TArtSystemOfUnit::MeV);
   rangeTable->Init(col);
   
   TCatDiffusedChargeResponseHelper *helper = new TCatDiffusedChargeResponseHelper;
   helper->SetDefaultIntegrateStep(0.1);
   helper->SetReadoutPadArray(&array);
   helper->SetResponseTableFileName("data/test.root");
   helper->SetDiffusionCoefficient(0.04);
   helper->SetReadoutPlaneOffset(-50.);
   helper->SetRangeTableHelper(&rangeTable); 
   helper->SetWorkFunction(wf);
   helper->SetAttachment(0);
   helper->Init(col);


   Double_t closest, rangeToEnd, angle, trackLength;
   
   helper->SetTrack(start,end);
   helper->GetResponse(id,charge,closest,angle,rangeToEnd, trackLength);

   

   printf("id = %d, q = %f, d = %f, r = %f\n",id,charge,closest,rangeToEnd);

   delete rangeTable;
   delete helper;
   delete proc;
   delete col;
}
   
void TCatDiffusedChargeResponseHelper::Test(const TVector3& start, const TVector3& end, Double_t step)
{

   Double_t wf = 37*eV; // eV
   Double_t dc = 0.04; // cm1/2
   dc = 0.000000004; // cm1/2
   Double_t rp = -50; // readoutplane offset

   TH1F *hBrag = new TH1F("hBrag","brag curve random number",100,0.,100.);
   // get pad array
   TCatReadoutPadArray *array = NULL;
   TCatReadoutPadConditionProcessor *proc = new TCatReadoutPadConditionProcessor;
   TEventCollection *col = new TEventCollection;
   proc->SetPadType(2);
   proc->Init(col);
   array = proc->GetPadArray();


   TRangeTableHelper *rangeTable = new TRangeTableHelper;
   rangeTable->SetFileName("data/DeuteronDeuterium/DeuteronDeuterium064.txt.out");
   rangeTable->SetTKECutoff(10.*TArtSystemOfUnit::MeV);   
   rangeTable->Init(col);

   
   TCatDiffusedChargeResponseHelper *helper = new TCatDiffusedChargeResponseHelper;
   helper->SetDefaultIntegrateStep(step);
   helper->SetReadoutPadArray(&array);
   helper->SetResponseTableFileName("data/test.root");   
//   helper->SetResponseTableFileName("data/restable_7mm_10um.root");
   helper->SetDiffusionCoefficient(dc);
   helper->SetReadoutPlaneOffset(rp);
   helper->SetAttachment(0.);
   helper->SetRangeTableHelper(&rangeTable);
   helper->SetWorkFunction(wf);
   helper->Init(col);

   TStopwatch watch;
   Double_t mctime;
   Double_t restime;
   TH2Poly *hPad = array->CreateHist("hPad","hPad",-50.,50.,-50.,50.);
   TH2Poly *hPad2 = array->CreateHist("hPad","hPad",-50.,50.,-50.,50.);
   art::TRandomGraph *randomBrag = new art::TRandomGraph(rangeTable->GetR2DE());
   {
      TVector3 track = end - start;
      TVector3 unitDirection = track.Unit();
      Double_t range = track.Mag();
      Double_t energy = rangeTable->GetR2E()->Eval(range);

      // check raw integral of brag curve
      Int_t nint = TMath::CeilNint(range / step);
      Double_t stepsize = range / nint;
      Double_t sum  = 0;
      Double_t sum2 = 0;
      Double_t sum3 = 0;
      Double_t sum4 = 0;
      Double_t sum5 = 0;
      for (Int_t i = 0; i < nint; i++) {
         Double_t dedx = rangeTable->GetR2DE()->Eval((i+0.5) * stepsize) * stepsize;
         sum += dedx;
         if (i*stepsize < 6) {
            sum2 += dedx;
         } else if (i*stepsize<12) {
            sum3 += dedx;
         } else if (i*stepsize<18) {
            sum4 += dedx;
         } else if (i*stepsize<24) {
            sum5 += dedx;
         }
      }
      printf("sum = %f (%.3f) for energy = %f, step = %f\n",sum,sum/energy*100,energy,stepsize);
      printf("dedx last (6mm)  = %f for energy = %f\n",sum2,rangeTable->GetR2E()->Eval(6));
      printf("dedx (12mm-6mm)  = %f for energy = %f\n",sum3,rangeTable->GetR2E()->Eval(12)-rangeTable->GetR2E()->Eval(6));
      printf("dedx (18mm-12mm) = %f for energy = %f\n",sum4,rangeTable->GetR2E()->Eval(18)-rangeTable->GetR2E()->Eval(12));
      printf("dedx (24mm-18mm) = %f for energy = %f\n",sum5,rangeTable->GetR2E()->Eval(24)-rangeTable->GetR2E()->Eval(18));
      
      
      Int_t numElectrons = TMath::FloorNint(energy / wf);
      Int_t nOrder = 1;
      watch.Start();
      for (Int_t i = 0; i < numElectrons * nOrder; ++i) {
         Double_t t = range - randomBrag->Generate(0,range);
//         Double_t t = range - gRandom->Uniform(0,range);
         Double_t x0 = start.X() + unitDirection.X() * t ;
         Double_t y0 = start.Y() + unitDirection.Y() * t  ;
         Double_t z0 = start.Z() + unitDirection.Z() * t ;
         Double_t y = y0 - rp ; // field cage offset;
         Double_t sigma = TMath::Sqrt(y/10) * dc *10.;
         Double_t x = gRandom->Gaus(x0,sigma);
         Double_t z = gRandom->Gaus(z0,sigma);
         hPad->Fill(x,z,1);
         hBrag->Fill(t,1);
      }
      hPad->Scale(1./nOrder);
      hPad->Draw();
      watch.Stop();
      mctime = watch.CpuTime();
   }

   for (Int_t istep = 0; istep < 20; istep ++) {
      TVector3 end2 = end;
      hPad2->ClearBinContents();
      // monte calro
      Double_t var = end.Z() + (istep - 10) * 0.1;
      end2.SetZ(var);
      TVector3 track = end - start;
      TVector3 unitDirection = track.Unit();
      Double_t range = track.Mag();
      Double_t energy = rangeTable->GetR2E()->Eval(range);
      Int_t numElectrons = TMath::FloorNint(energy / wf);
//      printf("numE = %d\n",numElectrons);

      Int_t nPads = hPad->GetNumberOfBins();
      Double_t total = 0;
      Double_t mctotal = 0;
      Double_t fcn = 0;
      helper->SetTrack(start,end2);
      Int_t nHits = 0;
      std::vector<Int_t> ids;
      std::vector<Double_t> mccharges;
      for (Int_t i=0; i<nPads; i++) {
         const Double_t mccharge = hPad->GetBinContent(i+1);
         if (mccharge < TMath::Limits<Double_t>::Epsilon()) continue;
         nHits++;
         ids.push_back(i);
         mccharges.push_back(mccharge);
      }
      watch.Start();
      for (Int_t i=0; i<nHits; i++) {
         Int_t id = ids[i];
         const Double_t mccharge = mccharges[i];
         Double_t charge;
         Double_t closest, rangeToEnd, angle, trackLength;
         helper->GetResponse(id,charge,closest,angle,rangeToEnd, trackLength);
         charge /= wf;
         total += charge;
         mctotal += mccharge;
         Double_t diff = charge - mccharge;
         Double_t fcn0 ;
         if (mccharge >  TMath::Limits<Double_t>::Epsilon()) {
            fcn0 = (diff * diff / (mccharge));
            fcn += (diff * diff / (mccharge));
         } else {
            fcn0 = (diff * diff / (charge));
            fcn += (diff * diff / (charge));
         }
//         Double_t trackLength = ((TCatReadoutPad*)array->At(id))->GetTrackLength(start,end2);
         printf("id = %3d, q = %10.4f, mc = %10.4f, len = %10.4f, d = %10.4f, r = %10.4f chi2 = %f\n",id,charge,mccharge,trackLength,closest,rangeToEnd,fcn0);
      }
      watch.Stop();
      restime = watch.CpuTime();
      printf("var = %10.4f total = %10.4f mctotal = %10.4f chi2 = %15.7f, nHits = %3d time = %f / %f for %d\n"
             ,var,total,mctotal,fcn,nHits,mctime,restime,numElectrons);
   }
   
   delete rangeTable;
   delete helper;
   delete proc;
   delete col;
}
   
void TCatDiffusedChargeResponseHelper::TestConvergenceFor(Int_t id, TVector3 start, TVector3 end, Double_t dc)
{
   // get pad array
   TCatReadoutPadArray *array = NULL;
   TCatReadoutPadConditionProcessor *proc = new TCatReadoutPadConditionProcessor;
   TEventCollection *col = new TEventCollection;
   Double_t wf = 37*eV; // eV
   proc->SetPadType(2);
   proc->Init(col);
   array = proc->GetPadArray();


   TRangeTableHelper *rangeTable = new TRangeTableHelper;
   rangeTable->SetFileName("data/DeuteronDeuterium/DeuteronDeuterium064.txt.out");
   rangeTable->SetTKECutoff(10.*TArtSystemOfUnit::MeV);
   rangeTable->Init(col);
   
   TCatDiffusedChargeResponseHelper *helper = new TCatDiffusedChargeResponseHelper;
   helper->SetDefaultIntegrateStep(0.1);
   helper->SetReadoutPadArray(&array);
   helper->SetResponseTableFileName("data/test.root");
   helper->SetDiffusionCoefficient(dc);
   helper->SetReadoutPlaneOffset(-50.);
   helper->SetRangeTableHelper(&rangeTable); 
   helper->SetWorkFunction(wf);
   helper->SetAttachment(0);
   helper->Init(col);


   Double_t charge,closest, angle, rangeToEnd, trackLength;
   const Int_t nstep = 7;
   Double_t steps[nstep] = {0.0001,0.001, 0.01, 0.1, 0.2, 0.5, 1.};
   for (Int_t istep = 0; istep < nstep; ++istep) {
      Double_t step = steps[istep];
      helper->SetDefaultIntegrateStep(step);   
      helper->SetTrack(start,end);
      helper->GetResponse(id,charge,closest,angle, rangeToEnd, trackLength);
      printf("step = %10.4f, q = %f, d = %f, r = %f\n",step,charge,closest,rangeToEnd);
   }

   


   delete rangeTable;
   delete helper;
   delete proc;
   delete col;
}




#endif


#if 0
void TCatDiffusedChargeResponseTest(Int_t id, const TVector3& start, const TVector3& end, Double_t &charge, Double_t &mccharge)
{
   // get pad array
   TCatReadoutPadArray *array = NULL;
   TCatReadoutPadConditionProcessor *proc = new TCatReadoutPadConditionProcessor;
   TEventCollection *col = new TEventCollection;
   Double_t wf = 37*eV; // eV
   proc->SetPadType(2);
   proc->Init(col);
   array = proc->GetPadArray();


   TRangeTableHelper *rangeTable = new TRangeTableHelper;
   rangeTable->SetFileName("data/DeuteronDeuterium/DeuteronDeuterium064.txt.out");
   rangeTable->SetTKECutoff(10.*TArtSystemOfUnit::MeV);
   rangeTable->Init(col);
   
   TCatDiffusedChargeResponseHelper *helper = new TCatDiffusedChargeResponseHelper;
   helper->SetDefaultIntegrateStep(0.1);
   helper->SetReadoutPadArray(&array);
   helper->SetResponseTableFileName("data/test.root");
   helper->SetDiffusionCoefficient(0.04);
   helper->SetReadoutPlaneOffset(-50.);
   helper->SetRangeTableHelper(&rangeTable); 
   helper->SetWorkFunction(wf);
   helper->SetAttachment(0);
   helper->Init(col);


   Double_t closest, rangeToEnd, angle, trackLength;
   
   helper->SetTrack(start,end);
   helper->GetResponse(id,charge,closest,angle,rangeToEnd, trackLength);

   

   printf("id = %d, q = %f, d = %f, r = %f\n",id,charge,closest,rangeToEnd);

   delete rangeTable;
   delete helper;
   delete proc;
   delete col;
}
#endif
