/**
 * @file   TTimeValidator.h
 * @brief  validate time or timestamp  in certain window
 *
 * @date   Created       : 2015-11-17 13:51:53 JST
 *         Last Modified : 2018-02-04 13:15:23 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_8CAEBE08_C83C_4841_AD95_99C5870E2CB6
#define INCLUDE_GUARD_UUID_8CAEBE08_C83C_4841_AD95_99C5870E2CB6

#include "TProcessor.h"

namespace art {
   class TTimeValidator;
}
class TClonesArray;

class art::TTimeValidator : public TProcessor {
public:
   TTimeValidator();
   virtual ~TTimeValidator();

   TTimeValidator(const TTimeValidator& rhs);
   TTimeValidator& operator=(const TTimeValidator& rhs);

protected:
   virtual void Init(TEventCollection *col);
   virtual void Process();
private:
   TString fInputName; // name of input collection
   TString fOutputName; // name of output collection
   TClonesArray **fInput; //! input
   TClonesArray  *fOutput; //! output

//   Int_t fTimeVariable; // 0 time 1 timestamp
   FloatVec_t fValidTimeRange;
   Float_t fValidTimeMin;
   Float_t fValidTimeMax;
   

   ClassDef(TTimeValidator,1) // validate time in certain window
};

#endif // INCLUDE_GUARD_UUID_8CAEBE08_C83C_4841_AD95_99C5870E2CB6
