/**
 * @file   TPPACAnodeTimingProcessor.h
 * @brief  extract timing from multiple PPAC anodes
 *
 * @date   Created       : 2016-05-25 12:04:18 JST
 *         Last Modified : 2018-02-10 04:17:37 JST (ota)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_A25592E8_99EC_4439_887D_7321899CDA4C
#define INCLUDE_GUARD_UUID_A25592E8_99EC_4439_887D_7321899CDA4C

#include "TProcessor.h"

namespace art {
   class TPPACAnodeTimingProcessor;
   class TPPACData;
   class TTimingData;
}

class TClonesArray;

class art::TPPACAnodeTimingProcessor : public TProcessor {
public:
   TPPACAnodeTimingProcessor();
   virtual ~TPPACAnodeTimingProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   typedef art::TPPACData       input_t;
   StringVec_t                  fInputColName;
   std::vector<TClonesArray**> *fInput; //!

   typedef art::TTimingData output_t;
   TString       fOutputName;
   TClonesArray *fOutput;

   TString    fParameterFile;
   FloatVec_t fTimingOffset;
   Bool_t     fUseTSumGate;

private:
   TPPACAnodeTimingProcessor(const TPPACAnodeTimingProcessor&);
   TPPACAnodeTimingProcessor& operator=(const TPPACAnodeTimingProcessor&);

   ClassDef(TPPACAnodeTimingProcessor,0) // extract timing from multiple PPAC anodes
};

#endif // INCLUDE_GUARD_UUID_A25592E8_99EC_4439_887D_7321899CDA4C
