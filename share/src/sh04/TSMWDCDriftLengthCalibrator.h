/**
 * @file   TSMWDCDriftLengthCalibrator.h
 * @brief
 *
 * @date   Created:       2013-12-25 13:33:29
 *         Last Modified: 2015-03-02 17:46:22 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TSMWDCDRIFTLENGTHCALIBRATOR_H
#define TSMWDCDRIFTLENGTHCALIBRATOR_H

#include <TProcessor.h>

namespace art {
   namespace sh04{
      class TSMWDCDriftLengthCalibrator;
   }
}

class TClonesArray;
class TVector3;

class art::sh04::TSMWDCDriftLengthCalibrator : public TProcessor {
public:
   // Default constructor
   TSMWDCDriftLengthCalibrator();
   virtual ~TSMWDCDriftLengthCalibrator();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputColName; // name of tracking result
   TString fOutputColName;    // name of output TMWDCHitData 
   TString fPlaneName;        // name of input TMWDCHitData

   TClonesArray **fTrackingResult;
   TClonesArray **fPlaneDataIn;
   TClonesArray *fPlaneDataOut;

   TVector3 *fPos; //!
   Double_t fZ;
   Double_t fCos;
   Double_t fSin;
   Double_t fCenter;
   Double_t fCellSize;

private:
   // Copy constructor (prohibited)
   TSMWDCDriftLengthCalibrator(const TSMWDCDriftLengthCalibrator&);
   // Assignment operator (prohibited)
   TSMWDCDriftLengthCalibrator& operator=(const TSMWDCDriftLengthCalibrator&);

   ClassDef(TSMWDCDriftLengthCalibrator,0) // processor for SMWDC drift-time drift-length calibration
};

#endif // TSMWDCDRIFTLENGTHCALIBRATOR_H
