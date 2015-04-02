/**
 * @file   TAxisTreeProj.h
 * @brief  Axis for tree projection
 *
 * @date   Created       : 2014-03-03 23:23:30 JST
 *         Last Modified : Mar 17, 2014 17:52:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_53035F56_59A2_467D_946C_E406B38A2605
#define INCLUDE_GUARD_UUID_53035F56_59A2_467D_946C_E406B38A2605

#include "TAxis.h"
#include "TAttCut.h"

#include <TTreeFormula.h>

namespace art {
   class TAxisTreeProj;
}

class TTreeFormula;

class art::TAxisTreeProj : public TAxis, public TAttCut {
public:
   TAxisTreeProj();
   TAxisTreeProj(const char* var, Int_t nbins, Double_t min, Double_t max, const char* cut = "");
   virtual ~TAxisTreeProj();

   TAxisTreeProj(const TAxisTreeProj& rhs);
   TAxisTreeProj& operator=(const TAxisTreeProj& rhs);

   virtual TObject* Clone(const char *newname = "") const;

   virtual Double_t EvalVariable(Int_t iData) { return fVariable->EvalInstance(iData); }
   virtual Double_t EvalSelection(Int_t iData) { return (fSelection)?fSelection->EvalInstance(iData):kTRUE; }

   virtual TTreeFormula* GetVariableFormula() { return fVariable; }
   virtual TTreeFormula* GetSelectionFormula() { return fSelection; }

   virtual void Print(Option_t *opt = "") const;

   virtual void SetVariableFormula(TTreeFormula* formula) { fVariable = formula; }
   virtual void SetSelectionFormula(TTreeFormula* formula) { fSelection = formula; }

   virtual Int_t GetNdata() {
      if (fSelection) {
         return TMath::Min(fVariable->GetNdata(),fSelection->GetNdata());
      } else {
         return fVariable->GetNdata();
      }
   }

protected:

private:
   TTreeFormula *fVariable;
   TTreeFormula *fSelection;

   ClassDef(TAxisTreeProj,1) // Axis for tree projection
};

#endif // INCLUDE_GUARD_UUID_53035F56_59A2_467D_946C_E406B38A2605
