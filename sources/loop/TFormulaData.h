/**
 * @file   TFormulaData.h
 * @brief  helper to generate data from formula
 *
 * @date   Created       : 2019-03-04 19:04:38 JST
 *         Last Modified : 2019-03-04 23:53:57 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_AC77C7E0_C4C6_4C96_A3E6_8567C160B545
#define INCLUDE_GUARD_UUID_AC77C7E0_C4C6_4C96_A3E6_8567C160B545

#include "TNamed.h"

namespace art {
   class TFormulaData;
}

class TTree;
class TTreeFormulaManager;
class TTreeFormula;
class TClonesArray;

class art::TFormulaData : public TNamed {
public:
   TFormulaData();
   virtual ~TFormulaData();

   TFormulaData(const TFormulaData& rhs);
   TFormulaData& operator=(const TFormulaData& rhs);

   virtual TClonesArray* CreateOutput(const char* name, const char* valueFormula, const char* selectFormula, TTree* tree);
   virtual Bool_t Sync();
   virtual void Process();

protected:
   TTree *fTree; //!
   TTreeFormulaManager *fManager; //!
   TClonesArray *fOutput; //!
   TTreeFormula* fValueFormula; //!
   TTreeFormula* fSelectFormula; //!
   

private:

   ClassDef(TFormulaData,1) // helper to generate data from formula
};

#endif // INCLUDE_GUARD_UUID_AC77C7E0_C4C6_4C96_A3E6_8567C160B545
