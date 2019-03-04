/**
 * @file   TFormulaDataGenerator.h
 * @brief  generate object from formula
 *
 * @date   Created       : 2019-03-04 01:05:49 JST
 *         Last Modified :2019-03-04 21:33:32 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_89371AAF_8F53_4D0B_86D9_C26227E368FC
#define INCLUDE_GUARD_UUID_89371AAF_8F53_4D0B_86D9_C26227E368FC

#include "TProcessor.h"

namespace art {
   class TFormulaDataGenerator;
   class TFormulaData;
}

class TTreeFormulaManager;
   class TTreeFormula;


class art::TFormulaDataGenerator : public TProcessor {
public:
   static const Int_t kNumTokenInDef;
   TFormulaDataGenerator();
   virtual ~TFormulaDataGenerator();

   TFormulaDataGenerator(const TFormulaDataGenerator& rhs);
   TFormulaDataGenerator& operator=(const TFormulaDataGenerator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();
protected:
   StringVec_t fDefinitions; // definitions of objects
   Int_t fNumDefs; // number of definitions
   std::vector<TFormulaData*> fFormulaDatas;
   std::vector<TClonesArray*> fOutputs;

private:
   TTree *fTree; //!

   ClassDef(TFormulaDataGenerator,1) // generate object from formula
};

#endif // INCLUDE_GUARD_UUID_89371AAF_8F53_4D0B_86D9_C26227E368FC
