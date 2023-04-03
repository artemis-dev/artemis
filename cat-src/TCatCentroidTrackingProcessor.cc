/**
 * @file   TCatCentroidTrackingProcessor.cc
 * @brief
 *
 * @date   Created       : 2016-04-19 12:16:50 JST
 *         Last Modified : 2016-11-11 23:37:28 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatCentroidTrackingProcessor.h"
#include <TCanvas.h>
#include <TVector3.h>
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TH2Poly.h>
#include <TH2F.h>
#include <TLine.h>
#include <TArtParticle.h>
#include <TSimpleData.h>
#include <TGraph.h>
#include <TEventHeader.h>
#include <TSystem.h>
#include <TEventDisplayProcessorHelper.h>
#include <TCatTrackResultPulse.h>
#include <TRangeTableHelper.h>
using art::TCatCentroidTrackingProcessor;

ClassImp(TCatCentroidTrackingProcessor)

TCatCentroidTrackingProcessor::TCatCentroidTrackingProcessor()
{
   RegisterInputCollection("InputCollection","pulse shapes involved in a track",fInputNameGuide,TString("athits"),
                           &fInputGuide,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterInputCollection("InputGuide","pulse shapes involved in a track",fInputNameAll,TString("atsub"),
                           &fInputAll,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterInputCollection("BeamMomentum","beam momentum",fNameBeamMomentum,TString("beamMomentum"),
                           &fBeamMomentum,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterInputCollection("BeamPosition","beam momentum",fNameBeamPosition,TString("beamPosition"),
                           &fBeamPosition,TClonesArray::Class_Name(),TVector3::Class_Name());
   RegisterInputCollection("BeamPosition","beam momentum",fNameBeamPosition,TString("beamPosition"),
                           &fBeamPosition,TClonesArray::Class_Name(),TVector3::Class_Name());
   RegisterInputCollection("EventHeader","Name of event header input",fNameEventHeader,TString("eventheader"),
                           &fEventHeader,art::TEventHeader::Class_Name());
   RegisterOutputCollection("EndPoint","Output end point",fOutputNameEndPoint,TString("endPoint"),
                            &fEndPoint,TVector3::Class_Name());
   RegisterOutputCollection("StartPoint","Output start point",fOutputNameStartPoint,TString("startPoint"),
                            &fStartPoint,TVector3::Class_Name());
   RegisterOutputCollection("FirstPadPos","Output first pad position",fOutputNameFirstPadPos,TString("firstPadPos"),
                            &fFirstPadPos,TVector3::Class_Name());
   RegisterOutputCollection("LastPadPos","Output last pad position",fOutputNameLastPadPos,TString("lastPadPos"),
                            &fLastPadPos,TVector3::Class_Name());
   RegisterOutputCollection("OutputHits","array of output pulse to be used for tracking",
                            fOutputName,TString("reshits"),
                            &fOutput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputCollection","Output array",fOutputNameTrack,TString("track"),
                           &fOutputTrack,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterOutputCollection("OutputDEDX","Output de/dx array",fOutputNameDEDX,TString("dedx"),
                            &fOutputDEDX,TClonesArray::Class_Name(),TCatTrackResultPulse::Class_Name());
   
   RegisterOutputCollection("Ex","excitation energy",fNameEx,TString("Ex"),
                            &fEx,TSimpleData::Class_Name());
   RegisterOutputCollection("ThetaCM","Scattering angle",fNameThetaCM,TString("ThetaCM"),
                            &fThetaCM,TSimpleData::Class_Name());
   RegisterInputInfo("ReadoutPadArray","name of readout pad array",fInfoNameReadoutPadArray,TString("readoutPad"),
                     &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
   RegisterOutputCollection("RangeMeasured","name of measured range",fNameRangeMeasured,TString("rangeMeasured"),
                            "D",&fRangeMeasured);
   RegisterOutputCollection("TKECalc","name of TKE calculated from range",fNameTKECalc,TString("TKECalc"),
                            "D",&fTKECalc);
   RegisterOutputCollection("TKEMeasured","name of TKE as sum of the pad on the trajectory",fNameTKEMeasured,TString("TKEMeasured"),
                            "D",&fTKEMeasured);

   fEventDisplayHelper = new TEventDisplayProcessorHelper("CentroidTracking","events for centroid tracking");
   fEventDisplayHelper->Register(this);

   fRangeTableHelper = new TRangeTableHelper;
   RegisterHelper(fRangeTableHelper);
   
   fHistXZ = NULL;
   fHistXY = NULL;
   fTrackXZ   = NULL;
   fTrackXY   = NULL;
}

TCatCentroidTrackingProcessor::~TCatCentroidTrackingProcessor()
{
   if (fHistXY) fHistXY->Delete();
   if (fHistXZ) fHistXZ->Delete();
   if (fTrackXZ)   fTrackXZ->Delete();
   if (fTrackXY)   fTrackXY->Delete();
}

TCatCentroidTrackingProcessor::TCatCentroidTrackingProcessor(const TCatCentroidTrackingProcessor& rhs)
{
}

TCatCentroidTrackingProcessor& TCatCentroidTrackingProcessor::operator=(const TCatCentroidTrackingProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TCatCentroidTrackingProcessor::Init(TEventCollection *col)
{
   if (!fReadoutPadArray) {
      SetStateError(Form("Cannot find readout pad array '%s'",fInfoNameReadoutPadArray.Data()));
      return;
   }

   fRange2EnergyTable = fRangeTableHelper->GetR2E();
   
   fHistXZ = (*fReadoutPadArray)->CreateHist("histXZ","histogram for hit pattern in xz",-50.,50.,-50.,50.);
   fHistXY = new TH2F("histXY","histogram for hit pattern in xy",100,-50.,50.,100,-50.,50.);
   fTrackXY = new TLine;
   fTrackXZ = new TLine;

   //======================================================================
   // Preparation for saving hit pattern

   if (fEventDisplayHelper->Displays()) {
      fEventDisplayHelper->Init(col);
      TPad *mainPad = fEventDisplayHelper->GetMainPad();
      mainPad->Clear("C");
      mainPad->Divide(1,2);
      mainPad->cd(1);
      fHistXZ->Draw("colz");
      fTrackXZ->Draw();
      mainPad->cd(2);
      fHistXY->Draw("colz");
      fTrackXY->Draw();
   }
}

void TCatCentroidTrackingProcessor::Process()
{
   fOutput->Clear("C");
   fOutputDEDX->Clear("C");
   fHistXZ->ClearBinContents();
   fHistXY->Reset();
   fEx->Invalidate();
   fEx->SetValue(TMath::QuietNaN());
   fThetaCM->Invalidate();
   fThetaCM->SetValue(TMath::QuietNaN());
   (*fRangeMeasured) = TMath::QuietNaN();
   (*fTKECalc) = TMath::QuietNaN();
   (*fTKEMeasured) = TMath::QuietNaN();
   (fEndPoint)->SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   (fStartPoint)->SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   
   Int_t nHits = (*fInputGuide)->GetEntriesFast();
//   Int_t nHits   = (*fInputAll)->GetEntriesFast();
   TClonesArray *input = *fInputGuide;
      

   if (!nHits) return;
   //======================================================================
   // find the most distant pair 
   Double_t maxDistance = 0;
   TCatPulseShape *maxPulse1 = NULL;
   TCatPulseShape *maxPulse2 = NULL;
   TCatPulseShape *pulse1 =  (TCatPulseShape*)input->UncheckedAt(0);
   for (Int_t iHit = 1; iHit != nHits; ++iHit) {
      TCatPulseShape *pulse =  (TCatPulseShape*)input->UncheckedAt(iHit);
      TVector3 &pos1 = pulse->GetPos();
      TVector3 &pos2 = pulse1->GetPos();
      Double_t distance = (pos1 - pos2).Mag();
      if (maxDistance < distance) {
         maxDistance = distance;
         maxPulse2 = pulse;
      }
   }
   maxDistance = 0;
   for (Int_t iHit = 0; iHit != nHits; ++iHit) {
      TCatPulseShape *pulse =  (TCatPulseShape*)input->UncheckedAt(iHit);
      TVector3 &pos1 = pulse->GetPos();
      TVector3 &pos2 = maxPulse2->GetPos();
      Double_t distance = (pos1 - pos2).Mag();
      if (maxDistance < distance) {
         maxDistance = distance;
         maxPulse1 = pulse;
      }
   }
   Double_t xend = TMath::Max(TMath::Abs(maxPulse1->GetX()),TMath::Abs(maxPulse2->GetX()));
   if (maxPulse1->GetX() < 0) xend *= -1;

   *fFirstPadPos = maxPulse1->GetPos();
   *fLastPadPos = maxPulse2->GetPos();

   //======================================================================
   // calculate the centroid of the hits
   Double_t w =0.;
   Double_t Xsum =0;
   Double_t Ysum =0;
   Double_t Zsum =0;
   Double_t XXsum =0;
   Double_t XYsum =0;
   Double_t XZsum =0;
   Double_t y0, z0, vx, vy,vz;
   for (Int_t i = 0; i != nHits; i++) {
      TCatPulseShape *pulse = (TCatPulseShape*)input->UncheckedAt(i);
      Double_t x = pulse->GetX();
      Double_t y = pulse->GetY();
      Double_t z = pulse->GetZ();
      Double_t c = pulse->GetCharge();
      w += c;
      Xsum += x * c;
      Ysum += y * c;
      Zsum += z * c;
      XXsum += x * x * c;
      XYsum += x * y * c;
      XZsum += x * z * c;
   }
   y0 = (XXsum * Ysum - XYsum * Xsum) / (w * XXsum -  Xsum * Xsum);
   z0 = (XXsum * Zsum - XZsum * Xsum) / (w * XXsum -  Xsum * Xsum);
   vy = (w * XYsum  - Xsum * Ysum) / (w * XXsum -  Xsum * Xsum);
   vz = (w * XZsum  - Xsum * Zsum) / (w * XXsum -  Xsum * Xsum);
   vx = xend;
   vy *= vx;
   vz *= vx;
   fEndPoint->SetXYZ(xend,vy+y0,vz+z0);
   fStartPoint->SetXYZ(0,y0,z0);
   TVector3 va(*fEndPoint-*fStartPoint);
   TVector3 &a1 = *fStartPoint;
   TVector3 &a2 = *fEndPoint;
   TVector3 start(50,0,0);
   // calculate brag curve
   Int_t num;
   std::vector<TVector3> intersections;
//   TClonesArray intersections(TVector3::Class_Name());
//   printf("***\n");
   (*fTKEMeasured) = 0.;
   for (Int_t i=0; i != nHits; i++) {
      TCatPulseShape *pulse = (TCatPulseShape*)input->UncheckedAt(i);
      TCatReadoutPad *pad = (TCatReadoutPad*)(*fReadoutPadArray)->UncheckedAt(pulse->GetID());
      pad->GetIntersection(*fStartPoint,*fEndPoint,intersections);
      num = intersections.size();
      if (num == 0 || num > 2) continue;

      
      TCatTrackResultPulse *out = (TCatTrackResultPulse*)fOutputDEDX->ConstructedAt(fOutputDEDX->GetEntriesFast());
      pulse->Copy(*out);
      TVector3 &pos = out->GetPos();
      pos = a2 - (a1 + (va.Unit() * (pos - a1)) * va.Unit());
      TVector3 p1, p2;
      p1 = intersections[0];
      if (TMath::Abs(start.X()) > TMath::Abs(p1.X())) {
         start = p1;
      }
      if (num  == 1) {
         if (pad->IsInside(fStartPoint->X(),fStartPoint->Z())) {
            p2 = *fStartPoint;
         } else if (pad->IsInside(fEndPoint->X(),fEndPoint->Z())) {
            p2 = *fEndPoint;
         }
      } else {
         p2 = intersections[1];
         if (TMath::Abs(start.X()) > TMath::Abs(p2.X())) {
            start = p2;
         }
      }

      out->SetTrackLength((p1-p2).Mag());
      out->SetDEDX(fRangeTableHelper->GetR2DE()->Eval(pos.Mag()));
      (*fTKEMeasured) += out->GetCharge();
//      printf("%10.3f %10.3f\n",out->GetCharge(),out->GetPos().Mag());
   }


   //======================================================================
   // draw histogram
   TArtParticle *beam = (TArtParticle*) (*fBeamMomentum)->UncheckedAt(0);
   TVector3 *beamPosition = (TVector3*) (*fBeamPosition)->UncheckedAt(0);   
   TVector3 track = (*fEndPoint) - (*fStartPoint);
   Double_t energy = fRange2EnergyTable->Eval(track.Mag());
   TArtParticle *p = (TArtParticle*)fOutputTrack->ConstructedAt(0);
   p->SetXYZT(vx,vy,vz,energy +1875.613);
   p->SetKineticEnergy(energy,1875.613);
   p->SetAngle(track.Theta(),track.Phi());
   beamPosition->SetY(fStartPoint->Y());
   beamPosition->SetZ(fStartPoint->Z());
   (*fRangeMeasured) = ((*fEndPoint) - start).Mag();
   (*fTKECalc) = fRange2EnergyTable->Eval(*fRangeMeasured);

   TArtParticle target;
   target.SetXYZT(0.,0.,0.,1875.613);

   TLorentzVector cm = (TLorentzVector)(*beam) + (TLorentzVector)target;
   
   TLorentzVector scat;
   scat = cm - *(TLorentzVector*)p;
   scat.Boost(-cm.BoostVector());
   fEx->SetValue(scat.M()-beam->M());
   fThetaCM->SetValue(scat.Theta());
   //======================================================================
   // draw histogram

   
   if (fEventDisplayHelper->Displays()) {
      for (Int_t iHit = 0; iHit != nHits; ++iHit) {
         TCatPulseShape *pulse = (TCatPulseShape*)input->UncheckedAt(iHit);
         fHistXZ->Fill(pulse->GetX(),pulse->GetZ(),pulse->GetCharge());
         fHistXY->Fill(pulse->GetX(),pulse->GetY(),pulse->GetCharge());
      }
      fTrackXZ->SetX1(0);
      fTrackXZ->SetY1(z0);
      fTrackXZ->SetX2(xend);
      fTrackXZ->SetY2(z0+vz);
      fTrackXY->SetX1(0);
      fTrackXY->SetY1(y0);
      fTrackXY->SetX2(xend);
      fTrackXY->SetY2(y0+vy);
      TPad *mainPad = fEventDisplayHelper->GetMainPad();
      mainPad->cd(1);
      fHistXZ->Draw("colz");
      fHistXZ->SetMinimum(1e-3);
      fTrackXZ->Draw();
      mainPad->cd(2);
      fHistXY->Draw("colz");
      fTrackXY->Draw();
      fEventDisplayHelper->Process();
   }
   
}
