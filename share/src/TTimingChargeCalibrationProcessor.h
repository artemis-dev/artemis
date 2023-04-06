/**
 * @file   TTimingChargeCalibrationProcessor.h
 * @brief
 *
 * @date   Created:       2013-11-06 20:31:13
 *         Last Modified: 2014-06-29 07:00:49 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

/* 
 * if no valid converter given, this processor does nothing.
 */

#ifndef TTIMINGCHARGECALIBRATIONPROCESSOR_H
#define TTIMINGCHARGECALIBRATIONPROCESSOR_H

#include <TProcessor.h>

#include <vector>

namespace art {
   class TTimingChargeCalibrationProcessor;
   class TConverterBase;
}

class TClonesArray;

class art::TTimingChargeCalibrationProcessor : public TProcessor {
public:
   // Default constructor
   TTimingChargeCalibrationProcessor();
   virtual ~TTimingChargeCalibrationProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString        fInputColName;
   TString        fOutputColName;
   TClonesArray **fInData; //!
   TClonesArray  *fOutData; //!

   TString        fTimingConverterArrayName;
   TString        fChargeConverterArrayName;
   std::vector<TConverterBase*> *fTimingConverterArray; //!
   std::vector<TConverterBase*> *fChargeConverterArray; //!
   Bool_t         fInputHasTiming;
   Bool_t         fInputHasCharge;
   Bool_t         fInputIsDigital;

private:
   // Copy constructor (prohibited)
   TTimingChargeCalibrationProcessor(const TTimingChargeCalibrationProcessor& rhs);
   // Assignment operator (prohibited)
   TTimingChargeCalibrationProcessor& operator=(const TTimingChargeCalibrationProcessor& rhs);

   ClassDef(TTimingChargeCalibrationProcessor,0) // processor for calibration of timing and charge data
};

#endif // TTIMINGCHARGECALIBRATIONPROCESSOR_H
