/* @file TCatTrackFindWithSiliconProcessor.cc
 * @brief track finding with silicon for ribf113
 *
 * @date Create        : 2019-05-16 20:37:00 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCatTrackFindWithSiliconProcessor.h"

#include "TH2F.h"
#include "TClonesArray.h"
#include "TCatTpcTrack.h"
#include "TCatReadoutPadArray.h"
#include "TEventCollection.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TH2Poly.h"

ClassImp(art::ribf113::TCatTrackFindWithSiliconProcessor)

using art::ribf113::TCatTrackFindWithSiliconProcessor;

TCatTrackFindWithSiliconProcessor::TCatTrackFindWithSiliconProcessor()
   : fHistHough(NULL), fHitPattern(NULL)
{
   RegisterInputCollection("Beam","name of beam track",fBeamName,TString("atbcls"),
                           &fInputBeam,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterInputCollection("Silicon","name of silicon hits",fSiliconName,TString("sihits"),
                           &fInputSilicon,TClonesArray::Class_Name(),IPosition::Class_Name());
   RegisterInputCollection("Recoil","name of recoil hits",fRecoilName,TString("atrpulse"),
                           &fInputRecoil,TClonesArray::Class_Name(),IPosition::Class_Name());
   RegisterOutputCollection("Output","name of output recoil hits",fOutputName,TString("atrcand"));
   RegisterOutputCollection("OutputTrack","name of output recoil hits",fOutputTrackName,TString("atrclscand"),
                            &fOutputTrack,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterProcessorParameter("NumBinsTheta","the number of bins for theta",fNumBinsTheta,45);
   RegisterProcessorParameter("NumBinsPhi","the number of bins for phi",fNumBinsPhi,120);
   RegisterProcessorParameter("MaxDistance","the maximum distance between candidate track and hit",
                              fMaxDistance,10.);
   RegisterInputInfo("readoutPad","the readout pad",fReadoutPadName,TString("readoutPad"),
                     &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
}

TCatTrackFindWithSiliconProcessor::~TCatTrackFindWithSiliconProcessor()
{
   if (fHistHough) {
      delete fHistHough;
      fHistHough = 0;
   }

   if (fHitPattern) {
      delete fHitPattern;
      fHitPattern = NULL;
   }
}

void TCatTrackFindWithSiliconProcessor::Init(TEventCollection *col)
{
   TDirectory *savedir = gDirectory;
   gROOT->mkdir(GetName())->cd();
   fHistHough = new TH2F("histHough","hough space",fNumBinsTheta,0.,180.,fNumBinsPhi,-180.,180.);
   fHitPattern = (*fReadoutPadArray)->CreateHist("hitPattern","hitPattern",-50.,50.,-50.,50.);
   savedir->cd();
   col->Add("glastHough",fHistHough,fOutputIsTransparent);
   col->Add("hitPattern",fHitPattern,fOutputIsTransparent);

   fOutput = new TClonesArray((*fInputRecoil)->GetClass());
   col->Add(fOutputName,fOutput,fOutputIsTransparent);
}

void TCatTrackFindWithSiliconProcessor::Process()
{
   fOutput->Clear("C");
   fOutputTrack->Clear("C");
   fHistHough->Reset();
   ((TH2Poly*)fHitPattern)->ClearBinContents();

   TClonesArray *beam = *fInputBeam;
   TClonesArray *silicon = *fInputSilicon;
   TClonesArray *recoil = *fInputRecoil;


   // check existence of input
   if (beam->GetEntriesFast() == 0 ||
       silicon->GetEntriesFast() == 0 ||
       recoil->GetEntriesFast() == 0) return;

   // y range restriction from silicon size
   const double si_dy = 45.;// mm

   for (Int_t ib = 0, nb = beam->GetEntriesFast(); ib < nb; ++ib) {
      TCatTpcTrack *btrack = static_cast<TCatTpcTrack*>(beam->UncheckedAt(ib));
      const TVector3& bdir = btrack->GetDirection();
      const TVector3& bpos = btrack->GetStartPoint();
      for (Int_t is = 0, ns = silicon->GetEntriesFast(); is < ns; ++is) {
         IPosition *sipos = dynamic_cast<IPosition*>(silicon->UncheckedAt(is));
         const TVector3& spos = sipos->Vec();
         const TVector3 sdir(0.,spos.X() < 0 ? 1. : -1.,0.);
         fHistHough->Reset();
         std::vector<IPosition*> validhits;
         for (Int_t ir = 0, nr = recoil->GetEntriesFast(); ir < nr; ++ir) {
            TDataObject *data = dynamic_cast<TDataObject*>(recoil->UncheckedAt(ir));
            if (data && !data->IsValid()) continue;

            IPosition *repos = dynamic_cast<IPosition*>(recoil->UncheckedAt(ir));
            const TVector3& rpos = repos->Vec();

            // reject hits at opposite side of silicon
            if ((rpos.X() - bpos.X()) * (spos.X() - rpos.X()) < 0) continue;
            // reject hits which have large Y position than silicon acceptance
            
            const double left = TMath::ATan(si_dy / TMath::Abs(spos.X()));
            const double right = TMath::ATan(TMath::Abs((rpos.Y() - bpos.Y())/(rpos.X() - bpos.X())));
//            printf("si_dy = %f, spos.X() = %f, rpos.Y() = %f, rpos.X() = %f, %f, %f\n",si_dy, spos.X(), rpos.Y(), rpos.X(),left,right);
            if (left < right) continue;
            
            validhits.push_back(repos);
            // beam-recoil normal vector
            const TVector3 brnvec = bdir.Cross(rpos-bpos);
            // silicon-recoil normal vector
            const TVector3 srnvec = sdir.Cross(spos-rpos);
            // direction of trajectory
            TVector3 dir = brnvec.Cross(srnvec);
//            if (dir.X() * (spos.X() - bpos.X()) < 0) {
//               dir *= -1;
//            }


            fHistHough->Fill(dir.Theta()*TMath::RadToDeg(),dir.Phi()*TMath::RadToDeg());
         }

         Int_t ibinx, ibiny, ibinz;
         fHistHough->GetMaximumBin(ibinx,ibiny,ibinz);
         Double_t theta = fHistHough->GetXaxis()->GetBinCenter(ibinx) * TMath::DegToRad();
         Double_t phi = fHistHough->GetYaxis()->GetBinCenter(ibiny) * TMath::DegToRad();
         
         TVector3 vertex;
         vertex.SetZ(spos.Z() - TMath::Abs(spos.X())/(TMath::Tan(theta) * TMath::Abs(TMath::Cos(phi)))) ;
         vertex.SetX(btrack->X(vertex.Z()));
         vertex.SetY(btrack->X(vertex.Z()));
         
         TVector3 direction;
         direction.SetPtThetaPhi(TMath::Abs(spos.X()),theta,phi);
         direction *= (spos.X()/direction.X());
//         printf("extract candidate\n");

         TCatTpcTrack *track = static_cast<TCatTpcTrack*>(fOutputTrack->ConstructedAt(fOutputTrack->GetEntriesFast()));
         track->Init();
         track->SetTrack(vertex,vertex+direction);

         
         for (std::vector<IPosition*>::iterator it = validhits.begin(), itend = validhits.end(); it != itend; ++it) {
            const TVector3& u = direction.Unit();
            const TVector3& p0 = vertex;
            const TVector3& p = (*it)->Vec();
            Double_t distance = TMath::Abs(u.Cross(p - p0).Mag()) ; // 1 = 1./u.Mag();
//            printf("distance = %f\n",distance);
            if (distance > 10.) continue;
//            (p-p0).Print();
            fHitPattern->Fill((*it)->X(),(*it)->Z());
            TObject *hits = dynamic_cast<TObject*>(*it);
            TCatPulseShape *output = (TCatPulseShape*)fOutput->ConstructedAt(fOutput->GetEntriesFast());
            TObject *trackoutput = track->NewHit();
            hits->Copy(*trackoutput);
            hits->Copy(*output);
//            output->SetXYZ(u.Cross(p-p0));
            
         }
      }
   }
}


