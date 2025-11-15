/**
 * @file   TMWDCCalibrationProcessor.h
 * @brief  Generates histograms for time to length Calibration of MWDC
 *
 * @date   Created:       2013-10-18 16:57:38
 *         Last Modified: 2014-06-29 06:11:30 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
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

#ifndef TMWDCCALIBRATIONPROCESSOR_H
#define TMWDCCALIBRATIONPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TMWDCCalibrationProcessor;
   class TGateObject;
   class TMWDCPlaneCalibrationTask;
}

class TClonesArray;
class TH1D;


class art::TMWDCCalibrationProcessor : public TProcessor {
public:
   TMWDCCalibrationProcessor();
   virtual ~TMWDCCalibrationProcessor();

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

   TH1D    *fH;      // not owned by this (do not delete)

   Bool_t                     fOwnsTask;
   TMWDCPlaneCalibrationTask *fTask;

   static const char* const kTASKNAME;

   Bool_t fUseMax;

private:
   // Copy constructor (prohibited)
   TMWDCCalibrationProcessor(const TMWDCCalibrationProcessor& rhs);
   // Assignment operator (prohibited)
   TMWDCCalibrationProcessor& operator=(const TMWDCCalibrationProcessor& rhs);

   ClassDef(TMWDCCalibrationProcessor,0) // processor for MWDC dx/dt calibration
};

#endif // TMWDCCALIBRATIONPROCESSOR_H
