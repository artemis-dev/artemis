/**
 * @file   TCatTrackDiffusedHelper.cc
 * @brief  helper for tracking with diffused charge
 *
 * @date   Created       : 2016-09-12 10:07:14 JST
 *         Last Modified : 2016-09-12 10:15:34 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTrackDiffusedHelper.h"

using art::TCatTrackDiffusedHelper;

ClassImp(TCatTrackDiffusedHelper)

TCatTrackDiffusedHelper::TCatTrackDiffusedHelper()
: fResponseTable(NULL), fDummyPulses(NULL)
{
}

TCatTrackDiffusedHelper::~TCatTrackDiffusedHelper()
{
   if (fResponseTable) {
      delete fResponseTable;
      fResponseTable = NULL;
   }
      
   if (fDummyPulses) {
      delete fDummyPulses;
      fDummyPulses = NULL;
   }
}

TCatTrackDiffusedHelper::TCatTrackDiffusedHelper(const TCatTrackDiffusedHelper& rhs)
{
}

TCatTrackDiffusedHelper& TCatTrackDiffusedHelper::operator=(const TCatTrackDiffusedHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TCatTrackDiffusedHelper::Register(TProcessor *proc)
{
   proc->RegisterProcessorParameter("Attachment","value of attachment coeeffient (1/cm)",fAttachment,0.);
   proc->RegisterProcessorParameter("FieldCageOffset","offset of the end of field cage respect to the beam axis",fFieldCageOffset,50.);
   proc->RegisterProcessorParameter("IntegrateStep","step of integration default = 0.5mm",fIntegrateStep,0.5);
   proc->RegisterProcessorParameter("FileNameResponseTable","path to the response table file",fResponseTableFileName,TString("path/to/file"));
   proc->RegisterInputInfo("ReadoutPad","name of readout pad array",fNameReadoutPadArray,TString("readoutPad"),
                           &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
}

void TCatTrackDiffusedHelper::Init(TEventCollection *col)
{
   LoadResponseTable(fResponseTableFileName);
   const Int_t nPad = (*fReadoutPadArray)->GetEntriesFast();
   fPadCenter.resize(nPad);
   fPadVertex.resize(nPad);
   for (Int_t i = 0; i < nPad; ++i) {
      TCatReadoutPad *pad = (TCatReadoutPad*)(*fReadoutPadArray)->UncheckedAt(i);
      fPadCenter[i].Set(pad->X(),pad->Y());
      fPadVertex[i].Set(pad->VertexX()[0], pad->VertexY()[0]);
   }
   fDummyPulses = new TClonesArray(TCatPulseShape::Class());
}

void TCatTrackDiffusedHelper::SetData(TObjArray *input)
{
   fDummyPulses->Clear("C");
   TCatTrackMinuitHelper::SetData(input);
//   return;
   const Double_t clusterRadius = 8.2;
   const Double_t xdistance = 2.5; // distance between x coordinate in the same column
   const Int_t nPads = (*fReadoutPadArray)->GetEntriesFast();
   std::vector<Int_t> used(nPads,0);
   std::vector<TCatPulseShape*> nullpulses;
   Double_t xmin = TMath::Abs(TMath::Limits<Double_t>::Max());
   Double_t xmax = TMath::Abs(TMath::Limits<Double_t>::Min());
   Double_t zmin = TMath::Limits<Double_t>::Max();
   Double_t zmax = TMath::Limits<Double_t>::Min();

   for (std::vector<TCatPulseShape*>::iterator it = fHits.begin(), end = fHits.end(); it != end; ++it) {
      TCatPulseShape* pulse = *it;
      const Double_t x = pulse->GetX();
      const Double_t z = pulse->GetZ();
      used[pulse->GetID()] = 1;
      if (TMath::Abs(x) < xmin) xmin = TMath::Abs(x);
      if (TMath::Abs(x) > xmax) xmax = TMath::Abs(x);
      if (z < zmin) zmin = z;
      if (z > zmax) zmax = z;
   }
   xmin = xmin - xdistance;
   xmax = xmax + clusterRadius;
   zmin -= clusterRadius;
   zmax += clusterRadius;

   printf("xmin = %5.3f, xmax = %5.3f, zmin = %5.3f, zmax = %5.3f\n",xmin,xmax,zmin,zmax);

   for (Int_t i=0; i<nPads; ++i) {
      const TCatReadoutPad* pad = static_cast<TCatReadoutPad*>((*fReadoutPadArray)->UncheckedAt(i));
      const Int_t    id = pad->GetID();
      const Double_t x = pad->X();
      const Double_t z = pad->Y();
      if (TMath::Abs(x) < xmin || TMath::Abs(x) > xmax || z < zmin || z > zmax || used[id]) {
         // out of range or already included 
         continue;
      }
      for (std::vector<TCatPulseShape*>::iterator it = fHits.begin(), end = fHits.end(); it != end; ++it) {
         TCatPulseShape* pulse = (*it);
         const Double_t x1 = pulse->GetX();
         const Double_t z1 = pulse->GetZ();
         if (TMath::Sqrt(TMath::Power(x - x1,2.)+TMath::Power(z - z1,2.)) > clusterRadius) {
            continue;
         }
         TCatPulseShape *dummy = static_cast<TCatPulseShape*>(fDummyPulses->ConstructedAt(fDummyPulses->GetEntriesFast()));
         dummy->SetID(id);
         dummy->SetCharge(0);
         dummy->GetPos().SetXYZ(x,pulse->GetY(),z);
         dummy->SetOffset(pulse->GetOffset());
         nullpulses.push_back(dummy);
         used[id] = 1;
         break;         
      }
   }
   std::copy(nullpulses.begin(),nullpulses.end(),std::back_inserter(fHits));
   
}


Double_t TCatTrackDiffusedHelper::CalcResidual(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz)
{
   // initialize
   Double_t fcn = 0.;
   Double_t fcnE = 0.;
   Double_t fcnY = 0.;
   // create utility vectors,
   const TVector3 va(vx,vy,vz); // direction
   const TVector3 unitDirection = va.Unit();
   const TVector3 trackStart(x0,y0,z0);
   const TVector3 trackEnd = trackStart + va;
   const Double_t range = (trackEnd-trackStart).Mag();
   
   const Int_t    nHits = fHits.size();
   const Double_t sigma = 1.; // resolution for y position
   // calculate step
   const Int_t nStep = TMath::CeilNint(range/fIntegrateStep);
   const Double_t step = range / nStep;
//   printf("nStep = %d step = %f range = %f\n",nStep,step,range);
   if (nStep < 0 || nStep > 100000) {
      Info("CalcResidual","nStep = %d for mag = %f",nStep,range);
      return (TMath::Limits<Double_t>::Max());
   }
   

   Double_t yEst = TMath::QuietNaN();
   Double_t yEst2 = TMath::QuietNaN(); // y estimated from the line on the center of the pad. The line is parallel to the track.

   // calculate total charge 
   Double_t totalCharge = 0.;
   // loop all the hit pulses
   for (Int_t iHit = 0; iHit < nHits; ++iHit)
   {
      const TCatPulseShape* const pulse = fHits[iHit];
      const TCatReadoutPad* const pad = (TCatReadoutPad*) (*fReadoutPadArray)->At(pulse->GetID());
      const Int_t id = pad->GetID();
//      if (!pad) {
//         Error("Eval","pad %p for pad id = %d\n",pad,pulse->GetID());
//         return 1e10; // large value. is this ok?
//      }
      totalCharge += pulse->GetCharge();      
      
      // calculate likelihood and add
      Double_t yExp = pulse->GetY();
      Double_t nExp = pulse->GetCharge() / fWorkFunction;
      // assume trackStart is the starting point of track measured
      Double_t driftLength = yExp - trackStart.Y() - fFieldCageOffset;
      // collection of attachment
      if (fAttachment > TMath::Limits<Float_t>::Epsilon()) {
         nExp /= TMath::Exp(- fAttachment * driftLength / TArtSystemOfUnit::cm);
      }
      // constant position resolution for drift length
      const TVector3 &center = pulse->GetPos();
      const TVector3 closestPoint = trackStart + ((center - trackStart) * unitDirection) * unitDirection;
      //Double_t closest = (center  - closestPoint).Mag();
      Double_t charge,closest,rangeToEnd;
      GetResponse(id, trackStart, unitDirection, range, nStep, step, charge, closest,rangeToEnd);
      const Double_t nEst = charge / fWorkFunction;
#if DEBUG
      printf("total range = %f\n",range);
//      printf("effective range from this pad = %f, length on this pad = %f (tracklength = %f)\n",range-start,length,tracklength);
      printf("exp : %f, est = %f\n",pulse->GetCharge(),charge);
#endif         

      totalCharge /= fWorkFunction;
      
//      fcn -= nExp * TMath::Log(nEst) - nEst - nExp * TMath::Log(nExp) + nExp;

//      Double_t sigma2 = 2.18e-3 / charge + 2.91e-3;
//      Double_t sigma2 = (1.43e-3 * charge + 7.86e-6)/fWorkFunction/fWorkFunction;
      Double_t sigma2 = (1.43e-3 * charge + 7.86e-6)/fWorkFunction/fWorkFunction;
      fcnE += (nExp - nEst) * (nExp-nEst) / sigma2;
//      fcnE += (nExp - nEst) * (nExp-nEst) / (nEst * nEst * sigma2);
//      fcnE += (nExp - nEst) * (nExp-nEst);
//      fcnE += closest*closest / (100) * (nExp/totalCharge);
      if (!TMath::IsNaN(yEst)) {
//         fcn +=(yExp - yEst) * (yExp - yEst) * 0.5 / sigma / sigma;
         fcnY +=(yExp - yEst) * (yExp - yEst) / sigma / sigma;
      } else if (!TMath::IsNaN(yEst2)) {
//         fcn +=(yExp - yEst2) * (yExp - yEst2) * 0.5 / sigma / sigma;
         fcnY +=(yExp - yEst2) * (yExp - yEst2)  / sigma / sigma;
      }         
      
   }
   fcn = fcnE + fcnY;
   if (fcn == TMath::Infinity() || fcn == TMath::QuietNaN() ) {
      fcn = TMath::Limits<Double_t>::Max();
   }
   return fcn;
}



void TCatTrackDiffusedHelper::GetResponse(Int_t id, const TVector3 &trackStart, const TVector3& unitDirection, const Double_t &range,
                                                Int_t nStep, Double_t step, Double_t &charge, Double_t &closest, Double_t& rangeToEnd)
{
   charge = 0;
   // center of pad
   const TVector2 &c2 = fPadCenter[id];
   // one vertex of pad
   const TVector2 &v1 = fPadVertex[id];
   // temporary to show the effective range
   Double_t start = 0;
   Double_t length = 0;
   closest = TMath::Limits<Double_t>::Max();
   for (Int_t i = 0; i != nStep; ++i) {
      const TVector3 rmid = trackStart + (i + 0.5) * step * unitDirection;
      // position of energy-loss segment to be calculated
      const TVector2 bmid(rmid.X(),rmid.Z());
      const TVector2 c2bmid = bmid-c2;
      const Double_t distance = c2bmid.Mod();
      // skip integration
      if (distance > 10.) continue;
      Double_t angle = TVector2::Phi_0_2pi(c2bmid.Phi() - (v1 - c2).Phi())/deg;
      angle -= TMath::Floor(angle/120) * 120 -30.;
//      angle -= TMath::Floor(angle/120) * 120 ;
      if (angle > 60.) angle = TMath::Abs(120 - angle);
      const Double_t drift = rmid.Y() - trackStart.Y() - fFieldCageOffset;
      Double_t x[] = {angle, distance, sqrt(drift/10.) * 0.04 * 10.};
      const Double_t weight =fResponseTable->Eval(x);
      if (TMath::IsNaN(weight)) {
         //printf("nan at step %d, angle = %f, distance = %f, drift = %f\n",i,angle, distance, drift);
         //rmid.Print();
         continue;
         // charge = TMath::QuietNaN();
      } else {
         const Double_t dedx = (fRange2EnergyLoss->Eval(range - i*step) +
                                fRange2EnergyLoss->Eval(range - (i +1)*step)) * 0.5;
         charge += step * dedx * weight;
         length += step * weight;
         if (start == 0 && weight > 0) {
            start = (rmid-trackStart).Mag();
         }
      }
      if (distance < closest) {
         closest = distance;
         rangeToEnd   = range - (i+0.5)*step;
      }
      
#if DEBUG
      if (weight > 0.001) {
//         printf("x = %f z = %f\n",bmid.X(),bmid.Y());
//         printf("i = %d / n = %d, angle = %f, distance = %f, drift = %f\n",i,nStep,angle,distance,drift);
//         printf("range = %f, dE = %f, step = %f, weight = %f\n",mag - (i + 0.5)*step,fRange2EnergyLoss->Eval(mag - (i + 0.5)*step), step, fResponseTable->Eval(x));
//         printf("%f\n",step * fRange2EnergyLoss->Eval(mag - (i + 0.5)*step) * fResponseTable->Eval(x));
      }
#endif            
   }
}

void TCatTrackDiffusedHelper::GetTrackResult(TCatTrackResult *result)
{
   if (!result) return;
   Double_t x0, y0, z0, range, vx, vy, vz;
   GetParameters(x0, y0,z0,vx, vy,vz, range);
   printf("GetTrackResult: %f %f %f \n",x0,y0,z0);
   const TVector3 trackStart(x0,y0,z0);
   const TVector3 va = range * TVector3(vx,vy,vz);
   const TVector3 trackEnd = trackStart + va;
   const TVector3 unitDirection = va.Unit();
   const Int_t    nStep = TMath::CeilNint(TMath::Abs(range)/fIntegrateStep);
   const Double_t step = TMath::Abs(range)/nStep;
   Double_t charge, closest, rangeToEnd;
   Double_t minRangeToEnd = TMath::Limits<Double_t>::Max();
   result->GetStartPoint() = trackStart;
   result->GetEndPoint() = trackEnd;
   result->CalculateRange();
   result->SetChi2(CalcResidual(x0,y0,z0,va.X(),va.Y(),va.Z()));
//   TClonesArray *hits = result->GetHits();


   Double_t e10 = 0;
   Double_t e30 = 0;
   Double_t r10max = 0;
   Double_t r30max = 0;
   
   const Int_t nHits = fHits.size();
   for (Int_t iHit = 0; iHit < nHits; ++iHit) {
      const TCatPulseShape* const pulse = fHits[iHit];
      const TCatReadoutPad* const pad = (TCatReadoutPad*) (*fReadoutPadArray)->At(pulse->GetID());
      TCatTrackResultPulse *out = result->CreateAndGetPulse();
      const Int_t id = pad->GetID();
      Double_t trackLength = TMath::QuietNaN();
      Double_t dedx = TMath::QuietNaN();
      Int_t num;
      TVector3* intersection;
      GetResponse(id,trackStart,unitDirection,TMath::Abs(range),nStep,step,charge,closest,rangeToEnd);
      pad->GetIntersection(trackStart,trackEnd,num,intersection);
      if (rangeToEnd < minRangeToEnd) minRangeToEnd = rangeToEnd;
      switch (num) {
      case 2: 
         trackLength = (intersection[0]-intersection[1]).Mag();
         {
            Double_t r1 = (trackEnd - intersection[0]).Mag();
            Double_t r2 = (trackEnd - intersection[1]).Mag();
            dedx = TMath::Abs(fRangeTableHelper->GetR2E()->Eval(r1) - fRangeTableHelper->GetR2E()->Eval(r2));
            
            if (r1 < 10 || r2 < 10.) {
               if (r10max < r1) r10max = r1;
               if (r10max < r2) r10max = r2;
               e10 += charge;
            }
            if (r1 < 30 || r2 < 30.) {
               if (r30max < r1) r30max = r1;
               if (r30max < r2) r30max = r2;
               e30 += charge;
            }
         }
         break;
      case 1:
         if (!pad->IsInside(trackEnd.X(),trackEnd.Z())) break;
         trackLength = (intersection[0] - trackEnd).Mag();
         dedx = fRangeTableHelper->GetR2E()->Eval(trackLength);
         if (r10max < trackLength) r10max = trackLength;
         if (r30max < trackLength) r30max = trackLength;
         e10 += charge;
         e30 += charge;
         break;
      default:
         break;
      }   
      pulse->Copy(*out);
      out->SetEnergyDeposit(charge);
      out->SetTrackLength(trackLength);
      out->SetDEDX(dedx);
      out->SetDistance(closest);
      out->SetRange(rangeToEnd);
   }

   result->SetDEDX10(e10/r10max);
   result->SetDEDX30(e30/r30max);
   result->SetCharge10(e10);
   result->SetCharge30(e30);
   result->SetRange10(r10max);
   result->SetRange30(r30max);
   result->SetDistanceToEnd(minRangeToEnd);
}

//======================================================================
//
Bool_t TCatTrackDiffusedHelper::LoadResponseTable(const char* filename)
{
   TDirectory *saved = gDirectory;
   TFile* fFile = TFile::Open(filename);
   saved->cd();
   TList *list = fFile->GetListOfKeys();
   Int_t n = list->GetEntries();
   TObject *obj = NULL;
   for (Int_t i=0; i!=n; i++) {
      const char* name = list->At(i)->GetName();
      obj = fFile->Get(name);
      if (obj->InheritsFrom(TNArray::Class())) {
         break;
      }
   }
   fResponseTable = (TNArray*) obj;
   if (fResponseTable) {
      fResponseTable->Load();
   }
   printf("TCatTrackDiffused::LoadResponseTable() Loaded\n");
   return kTRUE;
}


