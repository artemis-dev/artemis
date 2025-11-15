/**
 * @file   TCatGainEvaluationProcessor.h
 * @brief
 *
 * @date   Created       : 2014-07-17 19:22:28 JST
 *         Last Modified : 2018-02-04 13:09:01 JST (ota)
 * @author h307-3 <h307-3@ata01.atnet>
 *
 *    (C) 2014 h307-3
 */

#ifndef INCLUDE_GUARD_UUID_3339DDCD_611C_4556_9E0B_DA67E54E81F2
#define INCLUDE_GUARD_UUID_3339DDCD_611C_4556_9E0B_DA67E54E81F2

#include "TProcessor.h"

namespace art {
   class TCatGainEvaluationProcessor;
}

class TClonesArray;

class art::TCatGainEvaluationProcessor : public TProcessor {
public:
   TCatGainEvaluationProcessor();
   virtual ~TCatGainEvaluationProcessor();

   TCatGainEvaluationProcessor(const TCatGainEvaluationProcessor& rhs);
   TCatGainEvaluationProcessor& operator=(const TCatGainEvaluationProcessor& rhs);

protected:
   void Process();
private:
   TClonesArray **fInput; //!
   TClonesArray  *fOutput; //!

   TString fInputName;
   TString fOutputName;

   Int_t fIdMin;
   Int_t fIdMax;
   
   Float_t fOffsetMin;
   Float_t fOffsetMax;

   ClassDef(TCatGainEvaluationProcessor,1) //
};

#endif // INCLUDE_GUARD_UUID_3339DDCD_611C_4556_9E0B_DA67E54E81F2
