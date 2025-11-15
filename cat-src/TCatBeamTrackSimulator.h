/**
 * @file   TCatBeamTrackSimulator.h
 * @brief  simulate beam track
 *
 * @date   Created       : 2015-05-13 19:05:16 JST
 *         Last Modified : 2018-02-04 13:11:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_85C013AE_23BD_4183_A270_6230A37C6E19
#define INCLUDE_GUARD_UUID_85C013AE_23BD_4183_A270_6230A37C6E19

#include "TProcessor.h"

namespace art {
   class TCatBeamTrackSimulator;
   class TSimpleData;
   class TCatTpcTrack;
}

class TClonesArray;

class art::TCatBeamTrackSimulator : public TProcessor {
public:
   TCatBeamTrackSimulator();
   virtual ~TCatBeamTrackSimulator();

   TCatBeamTrackSimulator(const TCatBeamTrackSimulator& rhs);
   TCatBeamTrackSimulator& operator=(const TCatBeamTrackSimulator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   Parameter<DoubleVec_t> fStartPoint;
   Parameter<DoubleVec_t> fEndPoint;
   OutputData<TClonesArray,TCatTpcTrack> fOutput;
//   
//
//   
//   Float_t fP0;
//   Float_t fP1;
//
//   TSimpleData **fP0Data;
//   TSimpleData **fP1Data;
//
//   TString fInputNameP0;
//   TString fInputNameP1;
//
//   TString fOutputNameBeamMom;
//   TString fOutputNameBeamPos;
//
//   TClonesArray *fOutputArrayBeamMom; //!
//   TClonesArray *fOutputArrayBeamPos; //!
//
//   Int_t fMassNumber;
//   Int_t fAtomicNumber;
//   Float_t fIncidentEnergy;
//   Bool_t fSimulateMomentum;
   

   ClassDef(TCatBeamTrackSimulator,1) // simulate beam track
};

#endif // INCLUDE_GUARD_UUID_85C013AE_23BD_4183_A270_6230A37C6E19
