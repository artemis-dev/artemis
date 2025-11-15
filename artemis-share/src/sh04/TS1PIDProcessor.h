/**
 * @file   TS1PIDProcessor.h
 * @brief  processor for S1 PID
 *
 * @date   Created       : 2015-04-24 10:45:22 JST
 *         Last Modified : 2018-02-04 12:06:33 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_43ED13BF_589D_4132_A316_46C6F7D4283B
#define INCLUDE_GUARD_UUID_43ED13BF_589D_4132_A316_46C6F7D4283B

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TS1PIDProcessor;
      class TParticleIdentifier;
   }
   class TTimingData;
   class TTrack;
}

class TClonesArray;

class art::sh04::TS1PIDProcessor : public TProcessor {
public:
   TS1PIDProcessor();
   virtual ~TS1PIDProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString        fS0TrackName;
   TClonesArray **fS0TrackArray; //!
   TString        fS0DTrackName;
   TClonesArray **fS0DTrackArray; //!
   TString        fS1TrackName;
   TClonesArray **fS1TrackArray; //!
   TString        fTOFColName;
   TClonesArray **fTOFArray; //!
   TString        fNyokiName;
   TClonesArray **fNyokiArray; //!

   /* output */
   TString              fOutputPIDName;
   TParticleIdentifier *fPIDOut; //!
   TString              fOutputTOFName;
   TTimingData         *fTOFOut; //!
   TString              fOutputS1TrackName;
   TTrack              *fS1TrackOut; //!

private:
   TS1PIDProcessor(const TS1PIDProcessor&);
   TS1PIDProcessor& operator=(const TS1PIDProcessor&);

   ClassDef(TS1PIDProcessor,0) // processor for S1 PID
};

#endif // INCLUDE_GUARD_UUID_43ED13BF_589D_4132_A316_46C6F7D4283B
