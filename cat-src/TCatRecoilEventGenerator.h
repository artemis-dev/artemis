/**
 * @file   TCatRecoilEventGenerator.h
 * @brief  recoil event generator with diffusion
 *
 * @date   Created       : 2015-12-20 22:31:04 JST
 *         Last Modified : 2018-02-04 13:18:35 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5B7B09CA_DFE5_40B4_AA1F_24AFA6E69C4B
#define INCLUDE_GUARD_UUID_5B7B09CA_DFE5_40B4_AA1F_24AFA6E69C4B

#include "TProcessor.h"
#include <TCanvas.h>
#include "TCatDiffusedChargeResponseHelper.h"
#include "TCatFunctionChargeResidual.h"
#include "TEventDisplayProcessorHelper.h"

namespace art {
   class TCatRecoilEventGenerator;
   class TCatReadoutPadArray;
   class TRandomGraph;
   class TEventHeader;
   class TRangeTableHelper;
}

class TH1;

class TH2Poly;
class TGraph;
class TVector3;
class TClonesArray;

class art::TCatRecoilEventGenerator : public TProcessor {
public:
   TCatRecoilEventGenerator();
   virtual ~TCatRecoilEventGenerator();

   TCatRecoilEventGenerator(const TCatRecoilEventGenerator& rhs);
   TCatRecoilEventGenerator& operator=(const TCatRecoilEventGenerator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void BeginOfRun();
   virtual void PostLoop();

protected:

private:
   Int_t fNumLoop;
   
   TString fInputName;
   TClonesArray **fInput; //!

   TString fOutputName;
   TClonesArray *fOutput; //!

   TString fNameOutputTrackResult;;
   TClonesArray *fOutputTrackResult; //!

   TString fNameOutputTrackResultFolded;
   TClonesArray *fOutputTrackResultFolded; //!
   

   TRandomGraph *fRandomBrag; //!

   FloatVec_t fZRange;

   DoubleVec_t fPhiRange;


   TH2Poly *fHistPad; //!
   TH2Poly *fHistPadTime; //!
   Int_t fNumPads;

   TCanvas *fCanvas ; //!

   TString fNameEventHeader;
   TEventHeader *fEventHeader; //!
   TString fNameRunHeaders;
   TList   *fRunHeaders; // //!


   DoubleVec_t fBeamRegion;

   Double_t fChargeThreshold;

   TCatDiffusedChargeResponseHelper fChargeResponseHelper; //!   
   TCatFunctionChargeResidual fFunctionChargeResidual; //!

   DoubleVec_t fSigmaCoeff;
   DoubleVec_t fSigmaCoeffTrue;
   DoubleVec_t fSigmaCoeffTrueFolded;

   TEventDisplayProcessorHelper fEventDisplayHelper;

   std::vector<TH1*> fHists;
   
//   Float_t fThreshold; // threshold

   ClassDef(TCatRecoilEventGenerator,1) // recoil event generator with diffusion
};

#endif // INCLUDE_GUARD_UUID_5B7B09CA_DFE5_40B4_AA1F_24AFA6E69C4B
