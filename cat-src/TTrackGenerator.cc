/**
 * @file   TTrackGenerator.cc
 * @brief  Generate track from TArtParticle
 *
 * @date   Created       : 2018-05-23 14:19:58 JST
 *         Last Modified : 2018-08-28 10:49:42 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTrackGenerator.h"
#include <TClonesArray.h>
#include <TRangeTableHelper.h>
#include <TRandom3.h>
#include <TArtParticle.h>
#include <TCatTpcTrack.h>
#include <TGraph.h>

using art::TTrackGenerator;

ClassImp(TTrackGenerator)

const TTrackGenerator::Generator_t TTrackGenerator::kGenerator[art::TTrackGenerator::kNumTypes]  = { &TRandom::Gaus, &TRandom::Uniform };
const TString TTrackGenerator::kTypes[TTrackGenerator::kNumTypes] = { "Gauss", "Uniform" };

TTrackGenerator::TTrackGenerator()
   : fInput(NULL),fOutput(NULL),fRangeTable(NULL),fRandom(NULL)
{
   StringVec_t defaultStartPointDistribution(9,"0.");
   defaultStartPointDistribution[0] = "Gauss";
   defaultStartPointDistribution[1] = "Uniform";
   defaultStartPointDistribution[2] = "Uniform";
   
   // registration of parameters
   RegisterInputCollection("Input","name of input (TClonesArray of TArtParticle)",fInputName,TString("particle"),
                 &fInput,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterOutputCollection("Output","name of output (TClonesArray of art::TCatTpcTrack)",fOutputName,TString("track"),
                  &fOutput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterProcessorParameter("StartPointDistribution","parameters for calculation of start point distribution : Gauss or Uniform",
                              fStartPointDistribution,defaultStartPointDistribution);
   RegisterInputInfo("RangeTable","name of range table",fRangeTableName,TString("rangeTable"),
                     &fRangeTable,TRangeTableHelper::Class_Name());
}

TTrackGenerator::~TTrackGenerator()
{
   if (fRandom) delete fRandom;
}

TTrackGenerator::TTrackGenerator(const TTrackGenerator& rhs)
{
}

TTrackGenerator& TTrackGenerator::operator=(const TTrackGenerator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTrackGenerator::Init(TEventCollection *col)
{
   if (fStartPointDistribution.size() != kDimension * kParameters) {
      SetStateError(TString::Format("StartPointDistribuion should have 9 components: %d components indicated",
                                    (Int_t)fStartPointDistribution.size()));
      return;
   }
   // check the format and create generator
   TString error;
   for (Int_t i = 0; i < kDimension; ++i) {
      for (Int_t iType = 0; iType < kNumTypes; ++iType) {
         if (fStartPointDistribution[i*kParameters].EqualTo(kTypes[iType])) {
            fStartPointGenerator[i] = kGenerator[iType];
         }
      }
      if (!fStartPointGenerator[i]) {
         error += TString::Format("   Invalid type name of generator for %c : %s\n",
                                  (Char_t)('x'+i),fStartPointDistribution[i*kParameters].Data());
      }
   }
   if (!error.IsNull()) {
      SetStateError(error);
      return;
   }

   // generate objects at the end
   fRandom = new TRandom3;
}

void TTrackGenerator::Process()
{
   TClonesArray *input = (*fInput);
   // return without erro if no input 
   if (!input->GetEntriesFast()) return;

   TVector3 startPoint;
   for (Int_t iDim = 0; iDim < kDimension; ++iDim) {
      const Double_t v1 = fStartPointDistribution[kParameters * iDim + 1].Atof();
      const Double_t v2 = fStartPointDistribution[kParameters * iDim + 2].Atof();
      startPoint[iDim] = (fRandom->*fStartPointGenerator[iDim])(v1,v2);
   }

   for (Int_t iInput = 0, nInput = input->GetEntriesFast(); iInput < nInput; ++iInput) {
      TArtParticle *particle = static_cast<TArtParticle*>(input->UncheckedAt(iInput));
      Double_t mag = (*fRangeTable)->E2R(particle->TKE());
      TVector3 endPoint = startPoint + mag * particle->Vect().Unit();
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>(fOutput->ConstructedAt(iInput));
      track->SetTrack(startPoint,endPoint);
   }
}
   
