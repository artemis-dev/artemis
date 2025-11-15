/**
 * @file   TTpcEventGenerator.h
 * @brief  generate TPC event from track
 *
 * @date   Created       : 2018-05-24 15:29:03 JST
 *         Last Modified : 2018-05-25 17:24:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_468F2434_36B4_4E49_AE9B_2D39E1D8AF1B
#define INCLUDE_GUARD_UUID_468F2434_36B4_4E49_AE9B_2D39E1D8AF1B

#include "TProcessor.h"
#include <TCatFunctionChargeResidual.h>
#include <TCatDiffusedChargeResponseHelper.h>

namespace art {
   class TTpcEventGenerator;
   class TCatTpcTrack;
}

class TH2Poly;

class art::TTpcEventGenerator : public TProcessor {
public:
   TTpcEventGenerator();
   virtual ~TTpcEventGenerator();

   TTpcEventGenerator(const TTpcEventGenerator& rhs);
   TTpcEventGenerator& operator=(const TTpcEventGenerator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void GenerateEventFrom(TCatTpcTrack *track);

protected:

   TString fInputName; // name of input track
   TClonesArray **fInput; //! input clones array of track (TCatTpcTrack)

   TString fOutputPulsesName; // name of output pulses
   TClonesArray *fOutputPulses; //! output TClonesArray of TCatPulseShape

   TString fOutputTrackName; // name of output track (charge folded)
   TClonesArray *fOutputTrack; //! output TClonesArray of TCatTpcTrack (charge folded)

   Double_t fThreshold; // threshold for energy

   DoubleVec_t fBeamRegion; // beam region to be negrected

   DoubleVec_t fDefaultSigmaCoeff; // default sigma coeff
   DoubleVec_t fSigmaCoeffFold; // sigma coefficient for folding
   DoubleVec_t fFoldedSigmaCoeff; // folded sigma coefficient

   TCatFunctionChargeResidual fFunctionChargeResidual; //!
   TCatDiffusedChargeResponseHelper fChargeResponseHelper; //!

   TH2Poly *fHistCharge; //! histogram to accumulate charge


private:

   ClassDef(TTpcEventGenerator,1) // generate TPC event from track
};

#endif // INCLUDE_GUARD_UUID_468F2434_36B4_4E49_AE9B_2D39E1D8AF1B
