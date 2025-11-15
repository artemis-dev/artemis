/**
 * @file   TCatMissingMassProcessor.cc
 * @brief
 *
 * @date   Created       : 2016-07-24 22:43:18 JST
 *         Last Modified : 2018-02-04 13:21:33 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatMissingMassProcessor.h"
#include <TRangeTableHelper.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TClonesArray.h>
#include <TArtParticle.h>
#include <TCatTpcTrack.h>
#include <TGraph.h>


using art::TCatMissingMassProcessor;

ClassImp(TCatMissingMassProcessor)

Double_t mass;

TCatMissingMassProcessor::TCatMissingMassProcessor()
{

//   RegisterInputCollection("BeamInput","tracking result collection of beam", fNameBeamInput, TString("beamTrack"),
//                            &fBeamInput,TClonesArray::Class_Name(),TCatTrackResult::Class_Name());
   RegisterInputCollection("RecoilInput","tracking result collection of recoil", fNameRecoilInput, TString("recoilTrack"),
                            &fRecoilInput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterInputCollection("BeamInput","tracking result collection of beam", fNameRecoilInput, TString("beamTrack"),
                            &fBeamInput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterOutputCollection("RecoilOutput","tracking result collection of recoil", fNameRecoilOutput, TString("recoilMom"),
                            &fRecoilOutput,TClonesArray::Class_Name(),TArtParticle::Class_Name());

   RegisterOutputCollection("Ex","name of excitation energy",fNameEx, TString("Ex"),"D",&fEx);
   RegisterOutputCollection("ThetaCM","name of theta CM",fNameThetaCM, TString("ThetaCM"),"D",&fThetaCM);
   RegisterInputInfo("RangeTable","name of range table",fRangeTableName,TString("rangeTable"),
                           &fRangeTableHelper,TRangeTableHelper::Class_Name());
   RegisterProcessorParameter("MassNumber","Mass Number",fMassNumber,132);
   RegisterProcessorParameter("AtomicNumber","Atomic Number",fAtomicNumber,50);
   RegisterProcessorParameter("BeamEnergy","BeamEnergy (MeV/u)",fBeamEnergy,114.);
}

TCatMissingMassProcessor::~TCatMissingMassProcessor()
{
   
}

TCatMissingMassProcessor::TCatMissingMassProcessor(const TCatMissingMassProcessor& rhs)
{
}

TCatMissingMassProcessor& TCatMissingMassProcessor::operator=(const TCatMissingMassProcessor& rhs)
{
   if (this != &rhs) {

   }
    return *this;
}

void TCatMissingMassProcessor::Init(TEventCollection *col)
{
   Info("Init","Mass Number = %d, Atomic Number = %d, Beam Energy = %f",
        fMassNumber,fAtomicNumber,fBeamEnergy);
   Info("Init","Range Table File : %s",fRangeTableName.Data());

   new TGeoManager;
   TGeoElementTable *table = new TGeoElementTable;
   TGeoElementRN *element = table->GetElementRN(fMassNumber,fAtomicNumber);
   mass =  931.494061 * fMassNumber + element->MassEx();
   Info("Init","MassEx = %f MeV",element->MassEx());
}

void TCatMissingMassProcessor::Process()
{
   (*fEx) = TMath::QuietNaN();
   (*fThetaCM) = TMath::QuietNaN();
   fRecoilOutput->Clear("C");
   if (  // !(*fBeamInput)->GetEntriesFast() ||
       !(*fRecoilInput)->GetEntriesFast()) {
      return;
   }

   TCatTpcTrack *recoil = static_cast<TCatTpcTrack*>((*fRecoilInput)->UncheckedAt(0));
//   TCatTpcTrack *beam_track = static_cast<TCatTpcTrack*>((*fBeamInput)->UncheckedAt(0));
//   TCatTrackResult *beam   = static_cast<TCatTrackResult*>((*fBeamInput)->UncheckedAt(0));
   
   Double_t energy = (*fRangeTableHelper)->R2E(recoil->GetRange());
   const TVector3& startPoint = recoil->GetStartPoint();
   const TVector3& track = recoil->GetEndPoint() - startPoint;
   TArtParticle *p = static_cast<TArtParticle*>(fRecoilOutput->ConstructedAt(fRecoilOutput->GetEntriesFast()));
   p->SetXYZT(track.X(),track.Y(),track.Z(),energy + 1875.613);
   p->SetKineticEnergy(energy,1875.613);
   p->SetAngle(track.Theta(),track.Phi());

   TArtParticle target;
   target.SetXYZT(0.,0.,0.,1875.613);
   TArtParticle beam;
   beam.SetXYZT(0.,0.,1.,fBeamEnergy * fMassNumber + mass);
   beam.SetKineticEnergy(fBeamEnergy*fMassNumber,mass);

   TArtParticle& pRecoil = *p;


   TLorentzVector cm = static_cast<TLorentzVector>(beam) + static_cast<TLorentzVector>(target);
   TLorentzVector scat = cm - static_cast<TLorentzVector>(pRecoil);
   scat.Boost(-cm.BoostVector());
   (*fEx) = scat.M() - beam.M();
   (*fThetaCM) = scat.Theta();
                                                
      
}
