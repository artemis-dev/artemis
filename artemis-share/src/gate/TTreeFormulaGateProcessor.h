/**
 * @file   TTreeFormulaGateProcessor.h
 * @brief  gate generator using TTreeFormula
 *
 * @date   Created       : 2014-11-04 09:54:45 JST
 *         Last Modified : 2014-11-05 17:30:36 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9C81D14E_41DC_4AC3_96E2_89D002533C4D
#define INCLUDE_GUARD_UUID_9C81D14E_41DC_4AC3_96E2_89D002533C4D

#include "TProcessor.h"

class TTree;
class TTreeFormula;
class TTreeFormulaManager;

namespace art {
   class TGateArray;
   class TGateObject;
}

namespace art {
   class TTreeFormulaGateProcessor;
}

class art::TTreeFormulaGateProcessor : public TProcessor {
public:
   TTreeFormulaGateProcessor();
   virtual ~TTreeFormulaGateProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   TGateArray **fGateArray;
   TTree       *fTree;

   /* input */
   StringVec_t                fDefinitions;

   /* for processing */
   std::vector<TTreeFormula*>        fFormulae;
   std::vector<TTreeFormulaManager*> fManagers;

   /* output */
   std::vector<TGateObject*> fGates;

private:
   void PrepareTree(TEventCollection*);
   TString Substitute(TString,TGateArray*);
   TTreeFormulaGateProcessor(const TTreeFormulaGateProcessor&);
   TTreeFormulaGateProcessor& operator=(const TTreeFormulaGateProcessor&);

   ClassDef(TTreeFormulaGateProcessor,0) // gate generator using TTreeFormula
};

#endif // INCLUDE_GUARD_UUID_9C81D14E_41DC_4AC3_96E2_89D002533C4D
