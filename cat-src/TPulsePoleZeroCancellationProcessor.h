/**
 * @file   TPulsePoleZeroCancellationProcessor.h
 * @brief  pole zero cancellation
 *
 * @date   Created       : 2018-02-09 16:31:53 JST
 *         Last Modified : 2018-02-09 17:06:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_36FF9BC1_233F_4E1B_BFA7_6FC3F7B93C64
#define INCLUDE_GUARD_UUID_36FF9BC1_233F_4E1B_BFA7_6FC3F7B93C64

#include "TProcessor.h"

namespace art {
   class TPulsePoleZeroCancellationProcessor;
}

class TClonesArray;

class art::TPulsePoleZeroCancellationProcessor : public TProcessor {
public:
   TPulsePoleZeroCancellationProcessor();
   virtual ~TPulsePoleZeroCancellationProcessor();

   TPulsePoleZeroCancellationProcessor(const TPulsePoleZeroCancellationProcessor& rhs);
   TPulsePoleZeroCancellationProcessor& operator=(const TPulsePoleZeroCancellationProcessor& rhs);

   virtual void Process();

protected:
   TString fNameInput; // name of input collection
   TClonesArray** fInput; //! clones array (input collection)

   Double_t fTimeConstantPZ;
private:

   ClassDef(TPulsePoleZeroCancellationProcessor,1) // pole zero cancellation
};

#endif // INCLUDE_GUARD_UUID_36FF9BC1_233F_4E1B_BFA7_6FC3F7B93C64
