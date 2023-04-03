/**
 * @file   TCatBeamTrackSimulator.cc
 * @brief  simulate track
 *
 * @date   Created       : 2015-05-13 19:06:25 JST
 *         Last Modified : 2015-09-03 17:50:07 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatBeamTrackSimulator.h"
#include <TSimpleData.h>
#include <TVector3.h>
#include <TArtParticle.h>
#include <TClonesArray.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include "TCatTpcTrack.h"

using art::TCatBeamTrackSimulator;

ClassImp(TCatBeamTrackSimulator)

TCatBeamTrackSimulator::TCatBeamTrackSimulator()
//: fP0(0), fP1(1e20), fP0Data(NULL),fP1Data(NULL),fInputNameP0(""),fInputNameP1(""),
//   fOutputArrayBeamMom(NULL), fOutputArrayBeamPos(NULL)
{
//   RegisterProcessorParameter("DefaultP0","P0 input from outside",fP0,(Float_t)0.);
//   RegisterProcessorParameter("DefaultP1","P1 input from outside",fP1,(Float_t)TMath::Limits<Float_t>::Max());
//   RegisterInputCollection("InputP0","P0 input from outside",fInputNameP0,TString("p0"));
//   RegisterInputCollection("InputP1","P1 input from outside",fInputNameP1,TString("p1"));
//   RegisterOutputCollection("OutputBeamMomentum","beam particle",fOutputNameBeamMom,TString("beamMomentum"),
//                            &fOutputArrayBeamMom,TClonesArray::Class_Name(),TArtParticle::Class_Name());                            
//   RegisterOutputCollection("OutputBeamPosition","beam particle",fOutputNameBeamPos,TString("beamPosition"),
//                            &fOutputArrayBeamPos,TClonesArray::Class_Name(),TVector3::Class_Name());                            
//   RegisterProcessorParameter("MassNumber","mass number of beam particle",fMassNumber,132);
//   RegisterProcessorParameter("AtomicNumber","mass number of beam particle",fAtomicNumber,50);
//   RegisterProcessorParameter("IncidentEnergy","incident energy per nucleon (MeV/u)",fIncidentEnergy,(Float_t)100.);
//   RegisterProcessorParameter("SimulateMomentum","Track is assumed to be straight at X = 0 if true (default : false)",
//                              fSimulateMomentum,kFALSE);
   fOutput.SetDoAuto(true);
   Register(fStartPoint("StartPoint","coordinate of start point [x,y,z]",DoubleVec_t(3,0)));
   Register(fEndPoint("EndPoint","coordinate of start point [x,y,z]",DoubleVec_t(3,0)));
   Register(fOutput("Output","name of output track","aTrack"));
   
}

TCatBeamTrackSimulator::~TCatBeamTrackSimulator()
{
//   if (fOutputArrayBeamMom) delete fOutputArrayBeamMom;
//   if (fOutputArrayBeamPos) delete fOutputArrayBeamPos;
}

TCatBeamTrackSimulator::TCatBeamTrackSimulator(const TCatBeamTrackSimulator& rhs)
{
}

TCatBeamTrackSimulator& TCatBeamTrackSimulator::operator=(const TCatBeamTrackSimulator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatBeamTrackSimulator::Init(TEventCollection *col)
{
#if 0   
   if (fSimulateMomentum) {
      // imput will be ignored
      fP0Data = (TSimpleData**)col->GetObjectRef(fInputNameP0);
      fP1Data = (TSimpleData**)col->GetObjectRef(fInputNameP1);
      if (!fP0Data) {
         col->Add(fInputNameP0,new TSimpleData,fOutputIsTransparent);
         fP0Data = (TSimpleData**)col->GetObjectRef(fInputNameP0);
      }
      if (!fP1Data) {
         col->Add(fInputNameP1,new TSimpleData,fOutputIsTransparent);
         fP1Data = (TSimpleData**)col->GetObjectRef(fInputNameP1);
      }
      (*fP0Data)->SetValue(fP0);
      (*fP1Data)->SetValue(fP1);
      (*fP0Data)->Validate();
      (*fP1Data)->Validate();
   } else {
      // input should be given from outside
      fP0Data = (TSimpleData**)col->GetObjectRef(fInputNameP0);
      fP1Data = (TSimpleData**)col->GetObjectRef(fInputNameP1);
      if (!fP0Data || !fP1Data) {
         SetStateError(Form("Input is not given. Or SimulateMomentum should be true if you want to simulate straight line."));
         return;
      }
   }

   TArtParticle *beam = (TArtParticle*)fOutputArrayBeamMom->ConstructedAt(0);
   TVector3     *beamPosition = (TVector3*)fOutputArrayBeamPos->ConstructedAt(0);
   new TGeoManager;
   TGeoElementTable *table = new TGeoElementTable;
   TGeoElementRN *element = table->GetElementRN(fMassNumber,fAtomicNumber);
   Double_t mass =  931.494061 * fMassNumber + element->MassEx();
   printf("mass = %f\n",mass);
   beam->SetXYZT(0,0,1,fIncidentEnergy * fMassNumber + mass);
   beam->SetKineticEnergy(fIncidentEnergy * fMassNumber, mass);
   printf("theta = %f\n",beam->Theta());
   beamPosition->SetXYZ(fP0,0,0);
#endif   
}

void TCatBeamTrackSimulator::Process()
{
   fOutput->Clear("C");
   TCatTpcTrack *track = static_cast<TCatTpcTrack*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
   track->SetTrack(TVector3(&fStartPoint.fValue[0]),TVector3(&fEndPoint.fValue[0]));
   
#if 0   
   if (fSimulateMomentum) {
      // do nothing because straight line is assumed
      return;
   }
   TArtParticle *beam = (TArtParticle*)fOutputArrayBeamMom->ConstructedAt(0);
   TVector3     *beamPosition = (TVector3*)fOutputArrayBeamPos->ConstructedAt(0);
   Double_t p0 = (*fP0Data)->GetValue();
   Double_t p1 = (*fP0Data)->GetValue();
   Double_t x = -p0/p1;
   beam->SetTheta(TMath::ATan(-1./p1));
   beamPosition->SetXYZ(x,0,0);
#endif   
}
