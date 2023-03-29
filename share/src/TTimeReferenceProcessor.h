/**
 * @file   TTimeReferenceProcessor.h
 * @brief
 *
 * @date   Created:       2013-10-21 09:18:10
 *         Last Modified: 2014-07-01 11:52:02 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 * @note   adopted TCatV1190TimeReferenceProcessor
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMEREFERENCEPROCESSOR_H
#define TTIMEREFERENCEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TTimeReferenceProcessor;
   class TSegmentedData;
}

class art::TTimeReferenceProcessor : public TProcessor {
public:
   // Default constructor
   TTimeReferenceProcessor();
   virtual ~TTimeReferenceProcessor();

   virtual void Init(TEventCollection*);
   virtual void Process();

protected:
   TString          fInputColName;
   IntVec_t         fRefConfig;    //{devid, fp, det, geo, ch}
   IntVec_t         fSegConfig;    //{devid, fp, det, geo}
   TSegmentedData **fSegData;      //!

   Int_t GetTimeReference() const;
   void  SetTimeReference(Int_t);

private:
   // Copy constructor (prohibited)
   TTimeReferenceProcessor(const TTimeReferenceProcessor&);
   // Assignment operator (prohibited)
   TTimeReferenceProcessor& operator=(const TTimeReferenceProcessor&);

   ClassDef(TTimeReferenceProcessor,0) // processor for time reference treatment
};

#endif // TTIMEREFERENCEPROCESSOR_H
