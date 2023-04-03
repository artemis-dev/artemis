/* @file TCatTrackFittingWithSiliconProcessor.cc
 * @brief track fitting with silicon and beam hit
 *
 * @date Create        : 2019-05-21 11:19:32 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCatTrackFittingWithSiliconProcessor.h"
#include "TCatTpcTrack.h"
#include "TTimingChargePositionData.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "TROOT.h"


ClassImp(art::TCatTrackFittingWithSiliconProcessor)

using art::TCatTrackFittingWithSiliconProcessor;

TCatTrackFittingWithSiliconProcessor::TCatTrackFittingWithSiliconProcessor()
{
   fInputRecoil.SetDoAuto(true);
   fInputBeam.SetDoAuto(false);
   fInputSilicon.SetDoAuto(true);
   fOutput.SetDoAuto(true);
   
   Register(fInputRecoil("InputRecoil","name of recoil input","atrcls"));
   Register(fInputBeam("InputBeam","name of beam input","atbcls"));
   Register(fInputSilicon("InputSilicon","name of silicon input","sihit"));
   Register(fOutput("Output","name of track output","track"));
   Register(fSigmaCoeff("SigmaCoeff","[p0*p0 + p1*p1 * N + p2*p2 * N * N]",DoubleVec_t(3,0)));
   RegisterHelper(&fChargeResponseHelper);
   fFunctionChargeResidual.SetResponseHelper(&fChargeResponseHelper);
}

TCatTrackFittingWithSiliconProcessor::~TCatTrackFittingWithSiliconProcessor()
{
}

void TCatTrackFittingWithSiliconProcessor::Process()
{
   TClonesArray* recoils = *fInputRecoil.fData;
   TClonesArray* silicons = *fInputSilicon.fData;
   TClonesArray* outputs = fOutput.fData;
   outputs->Clear("C");

   if (recoils->GetEntriesFast() == 0
       || silicons->GetEntriesFast() == 0) return;
   
   //Info("Process","recoils # = %d",recoils->GetEntriesFast());
   //Info("Process","silicons # = %d",silicons->GetEntriesFast());

   TCatTpcTrack* candTrack = static_cast<TCatTpcTrack *>(recoils->UncheckedAt(0));
   TTimingChargePositionData *sihit = static_cast<TTimingChargePositionData*>(silicons->UncheckedAt(0));
   std::vector<TCatPulseShape*> hits;
   for (std::size_t i = 0, n = candTrack->GetHits().size(); i < n; ++i) {
      hits.push_back(candTrack->GetHits()[i]);
   }

   fFunctionChargeResidual.SetHits(hits);
   fFunctionChargeResidual.SetUseTotalCharge(false);
   fFunctionChargeResidual.SetUseRange(false);
   fFunctionChargeResidual.SetSigmaCoeff(fSigmaCoeff);
   
   Double_t additionalRange = fChargeResponseHelper.GetRangeTableHelper()->E2R(sihit->GetCharge());
   fChargeResponseHelper.SetAdditionalRange(additionalRange);
   

   const Double_t dx = sihit->X() - candTrack->GetStartPoint().X();
   const Double_t dy = dx / candTrack->GetDirection().X() * candTrack->GetDirection().Y();
   
   
   ROOT::Minuit2::Minuit2Minimizer min(ROOT::Minuit2::kSimplex);
   
   min.SetFunction(fFunctionChargeResidual);
   min.SetVariable(0,"x0",candTrack->GetStartPoint().X(),7.);
   min.SetVariable(1,"y0",candTrack->GetStartPoint().Y(),1);
   min.SetVariable(2,"z0",candTrack->GetStartPoint().Z(),7.);
   min.SetVariable(3,"vx",dx,0.11);
   min.SetVariable(4,"vy",dy,0.01);
   min.SetVariable(5,"vz",sihit->Z(),10.);

   min.FixVariable(0);
   min.FixVariable(3);
   min.FixVariable(1);
   min.FixVariable(4);
   min.FixVariable(5);

   Int_t savedLevel = gErrorIgnoreLevel;
   
   if (fVerboseLevel == 0 ) gErrorIgnoreLevel = 1001;
   min.Minimize();
#if 0   
   printf("z0 = %f vz = %f\n",min.X()[2],min.X()[5]);
   min.Minimize();
   printf("z0 = %f vz = %f\n",min.X()[2],min.X()[5]);
   min.Minimize();
   printf("z0 = %f vz = %f\n",min.X()[2],min.X()[5]);
#endif   
   gErrorIgnoreLevel = savedLevel;
   

   TVector3 startPoint(min.X());
#if 0   
   TVector3 direction(&(min.X()[3]));
   TVector3 endPoint = startPoint + (additionalRange + direction.Mag()) / direction.Mag() * direction;
#endif   
   TVector3 endPoint(&(min.X()[3]));
   TVector3 direction = endPoint - startPoint;
   endPoint =  startPoint + (additionalRange + direction.Mag()) / direction.Mag() * direction;
   
   TCatTpcTrack* aTrack = static_cast<TCatTpcTrack*>(outputs->ConstructedAt(outputs->GetEntriesFast()));
   aTrack->Clear("C");
   candTrack->Copy(*aTrack);
   aTrack->Validate();
   aTrack->Init();
   aTrack->SetChi2(fFunctionChargeResidual.DoEval(min.X()));
   aTrack->SetStatus(min.Status());
   aTrack->SetEDM(min.Edm());
   fChargeResponseHelper.SetAdditionalRange(0.);
   aTrack->SetResults(hits,startPoint,endPoint,&fChargeResponseHelper);
   
}

