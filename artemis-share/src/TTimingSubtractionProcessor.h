/**
 * @file   TTimingSubtractionProcessor.h
 * @brief
 *
 * @date   Created:       2013-11-07 15:17:35
 *         Last Modified: 2014-06-18 11:12:05 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMINGSUBTRACTIONPROCESSOR_H
#define TTIMINGSUBTRACTIONPROCESSOR_H

#include <TProcessor.h>
#include <vector>

namespace art {
   class TTimingSubtractionProcessor;
}

class TObjArray;
class TClonesArray;

class art::TTimingSubtractionProcessor : public TProcessor {
public:
   // Default constructor
   TTimingSubtractionProcessor();
   virtual ~TTimingSubtractionProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

   virtual void ProcessByTimeDiff();

protected:
   TString        fInputColName;
   TString        fOutputColName;
   TClonesArray **fInData;             //!
   TClonesArray  *fOutData; //!
   TClonesArray **fStartTimingData;    //!
   TString        fStartTimingColName;
   Int_t          fStartTimingDetID;
   Int_t         fUseMultipleStartTiming;
   Int_t         fMaxDetID;


   static const Int_t HITID_ORIGIN = 1;
   std::vector<Int_t> *fHitIDArray;    //!
   TObjArray          *fStartTiming; //!

   DoubleVec_t fValidTimeRange;
   Double_t fValidTimeMin;
   Double_t fValidTimeMax;

   Int_t fUseStart; // use start timing as timestamp if 1, or use end if 0
   

private:
   // Copy constructor (prohibited)
   TTimingSubtractionProcessor(const TTimingSubtractionProcessor&);
   // Assignment operator (prohibited)
   TTimingSubtractionProcessor& operator=(const TTimingSubtractionProcessor&);

   ClassDef(TTimingSubtractionProcessor,0) // processor for timing subtraction
};

#endif // TTIMINGSUBTRACTIONPROCESSOR_H
