/**
 * @file   TPulseShapeTimestampProcessor.h
 * @brief  generate timestamp using pulse shape
 *
 * @date   Created       : 2018-07-06 15:37:09 JST
 *         Last Modified : 2021-04-19 12:00:40 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_4F751E1D_47B6_4D61_821D_551328B041FF
#define INCLUDE_GUARD_UUID_4F751E1D_47B6_4D61_821D_551328B041FF

#include "TProcessor.h"

namespace art {
   class TPulseShapeTimestampProcessor;
   class TSimpleData;
   class TScalerData;
}

class art::TPulseShapeTimestampProcessor : public TProcessor {
public:
   TPulseShapeTimestampProcessor();
   virtual ~TPulseShapeTimestampProcessor();

   TPulseShapeTimestampProcessor(const TPulseShapeTimestampProcessor& rhs);
   TPulseShapeTimestampProcessor& operator=(const TPulseShapeTimestampProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void BeginOfRun();
   

protected:
   TString fInputName; // name of pulse shape input
   TClonesArray **fInput; //!

   TString fScalerInputName; // name of helper scaler input
   TScalerData **fScalerInput; //!

   TString fOutputName; // name of timestamp output
   TSimpleData *fOutput; //!

   Int_t fFastID; //! fast id
   
   Int_t fSlowID; 
   Double_t fSlowFrequency;
   
   Int_t fBits;
   Int_t fNumCycles;
   DoubleVec_t fFastSaved;
   Double_t fSlowSaved;
   DoubleVec_t fFastFrequency;
   DoubleVec_t fFirstTimestamp;
   DoubleVec_t fFirstTimestampSlow;
   DoubleVec_t fMaxTimeByFastClock;
   Int_t fMaxFastID; // maximum id of input pulse (zero origin)

private:

   ClassDef(TPulseShapeTimestampProcessor,1) // generate timestamp using pulse shape
};

#endif // INCLUDE_GUARD_UUID_4F751E1D_47B6_4D61_821D_551328B041FF
