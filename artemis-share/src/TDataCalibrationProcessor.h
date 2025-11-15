/**
 * @file   TDataCalibrationProcessor.h
 * @brief
 *
 * @date   Created:       2013-11-28 11:04:00
 *         Last Modified: 2014-06-18 11:40:44 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TDATACALIBRATIONPROCESSOR_H
#define TDATACALIBRATIONPROCESSOR_H

#include <vector>
#include <TProcessor.h>

namespace art {
   class TDataCalibrationProcessor;
   class TConverterBase;
   class TDataObject;
}

class TClass;
class TClonesArray;

class art::TDataCalibrationProcessor : public TProcessor {
public:
   // Default constructor
   TDataCalibrationProcessor();
   virtual ~TDataCalibrationProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t    fInputColName;
   TString        fOutputColName;
   TClonesArray **fInData; //!
   TClonesArray  *fOutData; //!

   Int_t          fDetIDOrigin;
   TString        fConverterArrayName;
   std::vector<TConverterBase*> *fConverterArray; //!
   Bool_t         fInputIsDigital;
   TString        fDataClassName;
   TClass        *fDataClass; //!

   Bool_t         fOverwrite;

   Int_t          fConverterArraySize;

private:
   template <typename T> void Calibrate(TDataObject*) const;
   Double_t ConvertVal(Double_t, Int_t) const;

   void (TDataCalibrationProcessor::*CalibrationMethod)(TDataObject*) const; //!
   void SetCalibrationMethod();

   // Copy constructor (prohibited)
   TDataCalibrationProcessor(const TDataCalibrationProcessor&);
   // Assignment operator (prohibited)
   TDataCalibrationProcessor& operator=(const TDataCalibrationProcessor&);

   ClassDef(TDataCalibrationProcessor,0) // processor for generic data calibration
};

#endif // TDATACALIBRATIONPROCESSOR_H
