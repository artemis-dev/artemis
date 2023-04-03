/**
 * @file   TChargeValidateProcessor.h
 * @brief  charge validation
 *
 * @date   Created       : 2014-06-25 17:07:19 JST
 *         Last Modified : 2018-02-04 13:08:43 JST (ota)
 * @author  <sharaq08@shg01.ribfdaq>
 *
 *    (C) 2014
 */

#ifndef INCLUDE_GUARD_UUID_A8CBAFE4_644C_4906_A7FB_D2E7E02D9A41
#define INCLUDE_GUARD_UUID_A8CBAFE4_644C_4906_A7FB_D2E7E02D9A41

#include "TProcessor.h"

namespace art {
   class TChargeValidateProcessor;
}

class TClonesArray;

class art::TChargeValidateProcessor : public TProcessor {
public:
   TChargeValidateProcessor();
   virtual ~TChargeValidateProcessor();

   TChargeValidateProcessor(const TChargeValidateProcessor& rhs);
   TChargeValidateProcessor& operator=(const TChargeValidateProcessor& rhs);
   void Init(TEventCollection *col);
   void Process();
protected:
   TClonesArray **fInput;  //!
   TClonesArray *fOutput; //!

   FloatVec_t fRange;
   TString fInputName;
   TString fOutputName;
   Bool_t  fOutputInvalid;


   
private:

   ClassDef(TChargeValidateProcessor,1) // charge validation
};

#endif // INCLUDE_GUARD_UUID_A8CBAFE4_644C_4906_A7FB_D2E7E02D9A41
