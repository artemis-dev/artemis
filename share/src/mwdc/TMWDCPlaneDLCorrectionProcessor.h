/**
 * @file   TMWDCPlaneDLCorrectionProcessor.h
 * @brief  MWDC Apply for the drift length correction parameter created by art::TMWDCDriftLengthCorrectionProcessor
 *
 * @date   Created       : 2018-07-18 13:13:24 JST
 *         Last Modified : 2019-11-15 10:19:20 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_4625F87E_F084_4E0A_BE65_1767D786164F
#define INCLUDE_GUARD_UUID_4625F87E_F084_4E0A_BE65_1767D786164F

#include "TProcessor.h"
#include <vector>
#include "TClonesArray.h"

namespace art {
   class TMWDCPlaneDLCorrectionProcessor;
   class TConverterBase;
}

class art::TMWDCPlaneDLCorrectionProcessor : public TProcessor {
public:
   TMWDCPlaneDLCorrectionProcessor();
   virtual ~TMWDCPlaneDLCorrectionProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   TString fInputColName;
   TClonesArray **fPlaneIn; //!
   StringVec_t fConverterList;
//   TString fConverterArrayName; 
   std::vector<TConverterBase*> *fConverterArray; //!
   Int_t fVerbose;
   Double_t fCellSize;

private:

   ClassDef(TMWDCPlaneDLCorrectionProcessor,1) // MWDC Apply for the drift length correction parameter created by art::TMWDCDriftLengthCorrectionProcessor
};

#endif // INCLUDE_GUARD_UUID_4625F87E_F084_4E0A_BE65_1767D786164F
