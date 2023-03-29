/**
 * @file   TCatTrackResultDisplayProcessor.h
 * @brief  helper for displaying the result of CAT tracking
 *
 * @date   Created       : 2016-07-24 12:04:49 JST
 *         Last Modified : 2016-12-11 11:13:57 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_E8BDFEF7_CA9F_4D00_878E_9811A65C5818
#define INCLUDE_GUARD_UUID_E8BDFEF7_CA9F_4D00_878E_9811A65C5818

#include "TEventDisplayProcessorHelper.h"
#include <TProcessor.h>

namespace art {
   class TCatTrackResultDisplayProcessor;
   class TCatTrackResult;
   class TCatReadoutPadArray;
}
class TLine;
class TGraph;
class TH2Poly;
class TH2D;
class TClonesArray;

class art::TCatTrackResultDisplayProcessor : public TProcessor {
public:
   TCatTrackResultDisplayProcessor();
   virtual ~TCatTrackResultDisplayProcessor();

   TCatTrackResultDisplayProcessor(const TCatTrackResultDisplayProcessor& rhs);
   TCatTrackResultDisplayProcessor& operator=(const TCatTrackResultDisplayProcessor& rhs);


   virtual void Init(TEventCollection *col);
   
   virtual void Process();

protected:
   StringVec_t fNamesInput; // name of input array
   std::vector<TClonesArray **> fInputs; //! vector of input array
   

   TString fNameReadoutPadArray;
   TCatReadoutPadArray **fReadoutPadArray; //!

   TH2Poly *fHistHitsZX; //!
   TH2D    *fHistHitsXY; //!
   TH2D *fHistHitsYZ; //!
   TLine   *fLineZX; //!
   TLine   *fLineXY; //!
   TLine   *fLineYZ; //!
   TH2D    *fEnergyCorrelation; //!

   TEventDisplayProcessorHelper *fEventDisplayHelper; //!
   

private:

   ClassDef(TCatTrackResultDisplayProcessor,1) // helper for displaying the result of CAT tracking
};

#endif // INCLUDE_GUARD_UUID_E8BDFEF7_CA9F_4D00_878E_9811A65C5818
