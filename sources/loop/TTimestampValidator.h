/**
 * @file   TTimestampValidator.h
 * @brief  validate timestamp by absolute value or relative to reference object
 *
 * @date   Created       : 2018-07-18 13:37:44 JST
 *         Last Modified : 2018-07-18 14:10:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_46597AB3_1FC4_47A7_9C5D_8E6F0C5412DB
#define INCLUDE_GUARD_UUID_46597AB3_1FC4_47A7_9C5D_8E6F0C5412DB

#include "TProcessor.h"

namespace art {
   class TTimestampValidator;
}

class art::TTimestampValidator : public TProcessor {
public:
   TTimestampValidator();
   virtual ~TTimestampValidator();

   TTimestampValidator(const TTimestampValidator& rhs);
   TTimestampValidator& operator=(const TTimestampValidator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputName;
   TClonesArray **fInput; //! input to be validate

   TString fReferenceName;
   TClonesArray **fReference; //! reference input

   TString fOutputName;
   TClonesArray *fOutput; //! output

   DoubleVec_t fSearchWindow; // search window

private:

   ClassDef(TTimestampValidator,1) // validate timestamp by absolute value or relative to reference object
};

#endif // INCLUDE_GUARD_UUID_46597AB3_1FC4_47A7_9C5D_8E6F0C5412DB
