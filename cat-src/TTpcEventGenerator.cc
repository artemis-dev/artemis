/**
 * @file   TCatTpcEventGenerator.cc
 * @brief  generate TPC event from track
 *
 * @date   Created       : 2018-05-23 20:20:28 JST
 *         Last Modified : 2018-10-03 05:05:57 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTpcEventGenerator.h"
#include "TCatTpcTrack.h"
#include "TCatTrackResultPulse.h"
#include <TH2Poly.h>
#include <TClonesArray.h>
#include "TArtMathUtil.h"
#include <TCatReadoutPad.h>
#include <TRandom.h>

using namespace art::TArtMathUtil;

using art::TTpcEventGenerator;

ClassImp(TTpcEventGenerator)



TTpcEventGenerator::TTpcEventGenerator()
: fDefaultSigmaCoeff(DoubleVec_t(3,0))
{
   RegisterInputCollection("Input","name of input track",fInputName,TString("track_true"),
                           &fInput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterOutputCollection("OutputPulses","name of output pulses",fOutputPulsesName,TString("mcpulse"),
                            &fOutputPulses,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputTrack","name of output track (folded charge)",fOutputTrackName,TString("track_folded"),
                            &fOutputTrack,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterProcessorParameter("Threshold","threshold for energy",fThreshold,0.);
   RegisterProcessorParameter("BeamRegion","beam region to be rejected from hits",fBeamRegion,DoubleVec_t(2,0));
   RegisterProcessorParameter("SigmaCoeffFold","SigmaCoeff for folding charge",fSigmaCoeffFold,DoubleVec_t(3,0));

   RegisterHelper(&fChargeResponseHelper);
}

TTpcEventGenerator::~TTpcEventGenerator()
{
}

TTpcEventGenerator::TTpcEventGenerator(const TTpcEventGenerator& rhs)
{
}

TTpcEventGenerator& TTpcEventGenerator::operator=(const TTpcEventGenerator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTpcEventGenerator::Init(TEventCollection *col)
{
   const Int_t nCoeff = 3;
   fFunctionChargeResidual.SetResponseHelper(&fChargeResponseHelper);
   fFunctionChargeResidual.SetSigmaCoeff(fDefaultSigmaCoeff);
   fFunctionChargeResidual.SetUseTotalCharge(false);
   fFunctionChargeResidual.SetRangeUnit(TArtSystemOfUnit::mm);

   // set default sigma coeff
   fDefaultSigmaCoeff[1] = 1.;
   fFoldedSigmaCoeff.resize(nCoeff);
   for (Int_t i = 0; i < nCoeff; ++i) {
      fFoldedSigmaCoeff[i] = TMath::Sqrt(fDefaultSigmaCoeff[i] * fDefaultSigmaCoeff[i] + fSigmaCoeffFold[i] * fSigmaCoeffFold[i]);
   }

   Double_t x1 = fChargeResponseHelper.GetActiveArea()[0];
   Double_t x2 = fChargeResponseHelper.GetActiveArea()[3];
   
   Double_t z1 = fChargeResponseHelper.GetActiveArea()[2];
   Double_t z2 = fChargeResponseHelper.GetActiveArea()[5];
   
   fHistCharge = fChargeResponseHelper.GetReadoutPadArray()->CreateHist("hPadCarge","pad charges",x1,x2,z1,z2);

}

void TTpcEventGenerator::Process()
{
   fHistCharge->ClearBinContents();
   fOutputPulses->Clear("C");
   fOutputTrack->Clear("C");
   TClonesArray *input = *fInput;
   for (Int_t iTrack = 0, nTrack = input->GetEntriesFast(); iTrack < nTrack; ++iTrack) {
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>(input->UncheckedAt(iTrack));
      GenerateEventFrom(track);
   }
}

   
                                                   
void TTpcEventGenerator::GenerateEventFrom(art::TCatTpcTrack *track)
{

// calculate range
   const TVector3 start = track->GetStartPoint();
   const TVector3 unit  = track->GetDirection().Unit();
   const TVector3 end  = track->GetEndPoint();
   const Double_t range = track->GetRange();
   const Int_t numPads = fChargeResponseHelper.GetNumPads();
   std::vector<Double_t> yvalue(numPads,TMath::Limits<Double_t>::Max());
   
   
   
   fChargeResponseHelper.SetTrack(start,end);
   Int_t nEl = fChargeResponseHelper.GetNumOfElectrons();
   TVector3 point;
   if (GetVerboseLevel() > 0) {
      Info("GenerateEventfrom","nEl = %d, energy = %f\n",nEl,fChargeResponseHelper.GetEnergyFrom(nEl));
   }
   for (Int_t iEl = 0; iEl < nEl; ++iEl) {
      fChargeResponseHelper.GeneratedElectronAt(point);
      if (IsWithinRange(point.X(),fBeamRegion[0],fBeamRegion[1])) continue;
      Int_t bin = fHistCharge->Fill(point.X(),point.Z());
      if (bin >= 0 && yvalue[bin-1] > point.Y()) {
         yvalue[bin-1] = point.Y();
      }
   }

   // create pulse
   std::vector<TCatPulseShape*> hits;
   for (Int_t i = 0; i!=numPads; ++i) {
      Double_t energy = fChargeResponseHelper.GetEnergyFrom(fHistCharge->GetBinContent(i+1));
      if (energy < fThreshold) continue;
      const TCatReadoutPad *pad = fChargeResponseHelper.GetReadoutPad(i);
      if (IsWithinRange(pad->X(),fBeamRegion[0],fBeamRegion[1])) continue;
      TCatPulseShape *pulse = static_cast<TCatPulseShape*>(fOutputPulses->ConstructedAt(fOutputPulses->GetEntriesFast()));
      pulse->SetID(i);
      pulse->SetCharge(energy);
      pulse->GetPos().SetXYZ(pad->X(),yvalue[i],pad->Y());
      pulse->Validate();
      hits.push_back(pulse);
   }

   const Double_t val[] = {start.X(), start.Y(), start.Z(), unit.X(), unit.Y(), unit.Z(), range};
   fFunctionChargeResidual.SetHits(hits);
   fFunctionChargeResidual.SetSigmaCoeff(fDefaultSigmaCoeff);
//   printf("%f %f %f\n",fDefaultSigmaCoeff[0],fDefaultSigmaCoeff[1],fDefaultSigmaCoeff[2]);
   track->Init();
   track->EstimateFromHits(hits);
   track->SetResults(hits,start,end,&fChargeResponseHelper);
   track->SetChi2(fFunctionChargeResidual.DoEval(val));

   // fold

   for (Int_t iHit = 0, nHits = fOutputPulses->GetEntriesFast(); iHit < nHits; ++iHit) {
      TCatPulseShape *pulse = static_cast<TCatPulseShape*>(fOutputPulses->UncheckedAt(iHit));
      const DoubleVec_t &sc = fSigmaCoeffFold;
      const Double_t ne = pulse->GetCharge()/fChargeResponseHelper.GetWorkFunction();
      const Double_t sigma = TMath::Sqrt(sc[0]*sc[0] + sc[1]*sc[1]*ne + sc[2]*sc[2]*ne*ne);
      pulse->SetCharge(gRandom->Gaus(ne,sigma) * fChargeResponseHelper.GetWorkFunction());
   }

   fFunctionChargeResidual.SetSigmaCoeff(fFoldedSigmaCoeff);
   TCatTpcTrack *folded = static_cast<TCatTpcTrack*>(fOutputTrack->ConstructedAt(fOutputTrack->GetEntriesFast()));
   folded->Init();
   folded->EstimateFromHits(hits);
   folded->SetResults(hits,start,end,&fChargeResponseHelper);
   folded->SetChi2(fFunctionChargeResidual.DoEval(val));
   
}
