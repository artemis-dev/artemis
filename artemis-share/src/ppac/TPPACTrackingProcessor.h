/**
 * @file   TPPACTrackingProcessor.h
 * @brief  ppac tracking processor
 *
 * @date   Created       : 2014-06-15 08:43:25 JST
 *         Last Modified : Jun 15, 2014 17:20:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2FAD5656_57B2_4889_AAA2_B735F07D8627
#define INCLUDE_GUARD_UUID_2FAD5656_57B2_4889_AAA2_B735F07D8627

#include "TProcessor.h"
#include <vector>

namespace art {
   class TPPACTrackingProcessor;
   class TPPACData;
   class TTrack;
}

class TClonesArray;

class art::TPPACTrackingProcessor : public TProcessor {
public:
   TPPACTrackingProcessor();
   virtual ~TPPACTrackingProcessor();

   TPPACTrackingProcessor(const TPPACTrackingProcessor& rhs);
   TPPACTrackingProcessor& operator=(const TPPACTrackingProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void Track(std::vector<art::TPPACData*> hits, TTrack *track);
   
protected:
   std::vector<TClonesArray**> fInputs; //!
   TClonesArray *fOutput; //!

   StringVec_t fInputColNames; // name of input colletions
   TString fOutputColName; // name of output collections

   Int_t fNumPlane; // the number of planes to be used to track

   
private:

   ClassDef(TPPACTrackingProcessor,1) // ppac tracking processor
};

#endif // INCLUDE_GUARD_UUID_2FAD5656_57B2_4889_AAA2_B735F07D8627
