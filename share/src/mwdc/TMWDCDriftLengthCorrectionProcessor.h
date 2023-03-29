/**
 * @file   TMWDCDriftLengthCorrectionProcessor.h
 * @brief  make drift length correction coefficient file
 *
 * @date   Created       : 2018-07-13 17:59:38 JST
 *         Last Modified : 2020-05-27 15:03:05 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_AAB955F8_FA28_4D55_B77C_AB48AFE82F5D
#define INCLUDE_GUARD_UUID_AAB955F8_FA28_4D55_B77C_AB48AFE82F5D

#include <TProcessor.h>

#include "TMWDCPlaneCalibrationTask.h"
// #include "TEventDisplayProcessorHelper.h"
#include "TH2D.h"
#include "TPad.h"
namespace art {
   class TMWDCDriftLengthCorrectionProcessor;
   class TMWDCPlaneCalibratonTask;
}
class TClonesArray;
class TH1D;

class art::TMWDCDriftLengthCorrectionProcessor : public TProcessor {
public:
   TMWDCDriftLengthCorrectionProcessor();
   virtual ~TMWDCDriftLengthCorrectionProcessor();

   TMWDCDriftLengthCorrectionProcessor(const TMWDCDriftLengthCorrectionProcessor& rhs){}
   TMWDCDriftLengthCorrectionProcessor& operator=(const TMWDCDriftLengthCorrectionProcessor& rhs){return *this;}

   virtual void Init(TEventCollection *col);
   virtual void BeginOfRun();
   virtual void Process();
   
protected:
   TString fInputTrack;
   TString fInputPlane1;
   TString fInputPlane2;
   TClonesArray **fTrackIn; //!
   TClonesArray **fPlaneIn1; //!
   TClonesArray **fPlaneIn2; //!
   Double_t fCellSize;

   Int_t fVerbose; //!
   
   TH1D *fH; //!
   TH1D *fH1; //!
   TH1D *fH2; //!

   TH2D *fBlankHist; //!
   
   // TEventDisplayProcessorHelper *fEventDisplayHelper; //!
   TPad *fPad; //->
   static const char* const kTASKNAME;  //!

   Bool_t fOwnsTask;
   TMWDCPlaneCalibrationTask *fTask; //->
   
private:
   
   ClassDef(TMWDCDriftLengthCorrectionProcessor,1) //
};

#endif // INCLUDE_GUARD_UUID_AAB955F8_FA28_4D55_B77C_AB48AFE82F5D
