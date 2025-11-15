/**
 * @file   TTrackGenerator.h
 * @brief  Generate track from TArtParticle
 *
 * @date   Created       : 2018-05-23 14:19:23 JST
 *         Last Modified : 2018-05-23 16:16:16 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_591352F0_5CE1_4D37_89DD_0DE177CF6C8A
#define INCLUDE_GUARD_UUID_591352F0_5CE1_4D37_89DD_0DE177CF6C8A

#include "TProcessor.h"
#include <TRandom.h>
namespace art {
   class TTrackGenerator;
   class TRangeTableHelper;
}

class TClonesArray;

class art::TTrackGenerator : public TProcessor {
public:
   // typedef of random generator
   typedef Double_t (TRandom::*Generator_t)(Double_t,Double_t);
   static const Int_t kDimension = 3;
   static const Int_t kParameters = 3;
   static const Int_t kNumTypes = 2;
   static const TString kTypes[kNumTypes];
   static const Generator_t kGenerator[kNumTypes];
   
   TTrackGenerator();
   virtual ~TTrackGenerator();

   TTrackGenerator(const TTrackGenerator& rhs);
   TTrackGenerator& operator=(const TTrackGenerator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:

   TString fInputName; // name of input particle
   TClonesArray **fInput; //! array of input particle
   TString fOutputName;  // name of output particle
   TClonesArray *fOutput; //! array of output particle

   TString fRangeTableName; // name of range table helper
   TRangeTableHelper **fRangeTable; //! range table helper

   TRandom *fRandom; //! random generator

   StringVec_t fStartPointDistribution; // parameters for calculation of start point distribusion

   Generator_t fStartPointGenerator[kDimension]; //! start point random generator

private:

   ClassDef(TTrackGenerator,1) // Generate track from TArtParticle
};

#endif // INCLUDE_GUARD_UUID_591352F0_5CE1_4D37_89DD_0DE177CF6C8A
