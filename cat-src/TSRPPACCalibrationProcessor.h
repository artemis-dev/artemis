/**
 * @file   TSRPPACCalibrationProcessor.h
 * @brief  Generates histograms for time to length Calibration of SRPPAC
 * Based on TMWDCCalibrationProcessor by Kawase
 *
 * @date   Created:       2013-10-18 16:57:38 
 *         Last Modified: 2019-08-30 18:52:20 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2019 OTA Shinsuke All rights reserved
 */

/* # usage
 *
 * To generate time-to-length parameter,
 *  1. open TBrowser
 *  2. browse artemis/loops/loop0/mwdccalib
 *  3. right-click tasks and choose ExecuteTasks
 *
 *
 * # about parameters
 *
 * parameter:
 *   # output of TTimingChargeMappingProcessor
 *   - name:  InputCollection
 *     value: mwdc1_x
 *   # definition of valid range for hit timing
 *   # data shall not be filled if its timing is out of range
 *   - name: TimingGate
 *     value: [-1000,0]		
 *   # definition of valid range for charge
 *   # data shall not be filled if its charge is out of range
 *   - name: ChargeGate		
 *     value: [600,1400]		
 */

#ifndef TSRPPACCALIBRATIONPROCESSOR_H
#define TSRPPACCALIBRATIONPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TSRPPACCalibrationProcessor;
   class TGateObject;
   class TSRPPACPlaneCalibrationTask;
}

class TClonesArray;
class TH1D;


class art::TSRPPACCalibrationProcessor : public TProcessor {
public:
   TSRPPACCalibrationProcessor();
   virtual ~TSRPPACCalibrationProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void BeginOfRun();

protected:
   TString     fInputColName;
   TClonesArray **fPlaneIn; //! not owned by this
   FloatVec_t  fTimingGate;
   FloatVec_t  fChargeGate;
   Bool_t      fHasTimingGate;
   Bool_t      fHasChargeGate;
   TString     fGateName;
   TGateObject **fGate;
   Int_t       fCombination;

   TH1D    *fH;      // not owned by this (do not delete)

   Bool_t                     fOwnsTask;
   TSRPPACPlaneCalibrationTask *fTask;

   static const char* const kTASKNAME;

   Bool_t fUseMax;

private:
   // Copy constructor (prohibited)
   TSRPPACCalibrationProcessor(const TSRPPACCalibrationProcessor& rhs);
   // Assignment operator (prohibited)
   TSRPPACCalibrationProcessor& operator=(const TSRPPACCalibrationProcessor& rhs);

   ClassDef(TSRPPACCalibrationProcessor,0) // processor for SRPPAC dx/dt calibration
};

#endif // TSRPPACCALIBRATIONPROCESSOR_H
