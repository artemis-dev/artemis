/**
 * @file   TFormulaData.cc
 * @brief  helper to generate formula data
 *
 * @date   Created       : 2019-03-04 19:07:58 JST
 *         Last Modified :2019-03-04 23:08:58 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shinsuke OTA
 */

#include "TFormulaData.h"
#include "TTree.h"
#include "TTreeFormulaManager.h"
#include "TTreeFormula.h"
#include "TSimpleData.h"
#include "TClonesArray.h"

using art::TFormulaData;

ClassImp(TFormulaData)

TFormulaData::TFormulaData()
: fManager(NULL), fOutput(NULL), fValueFormula(NULL), fSelectFormula(NULL)
{
}

TFormulaData::~TFormulaData()
{
}

TFormulaData::TFormulaData(const TFormulaData& rhs)
{
}

TFormulaData& TFormulaData::operator=(const TFormulaData& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

TClonesArray* TFormulaData::CreateOutput(const char* name, const char* valueFormula, const char* selectFormula, TTree* tree)
{
   fTree = tree;
   fManager = new TTreeFormulaManager;
   fValueFormula = new TTreeFormula(Form("V_%s",name), valueFormula, tree);
   fManager->Add(fValueFormula);
   if (!TString(selectFormula).IsNull()) {
      fSelectFormula = new TTreeFormula(Form("S_%s",name), selectFormula, tree);
      fManager->Add(fSelectFormula);
   }
   fOutput = new TClonesArray(TSimpleData::Class());
   SetName(name);
   SetTitle(Form("%s {%s}",valueFormula,selectFormula));
   return fOutput;
}

Bool_t TFormulaData::Sync()
{
   return fManager->Sync(); 
}


void TFormulaData::Process()
{
   fOutput->Clear("C");
   Int_t nData = fManager->GetNdata();
   for (Int_t iData = 0; iData < nData; ++iData) {
      if (fSelectFormula && !fSelectFormula->EvalInstance(iData)) continue;
      TSimpleData* data = (TSimpleData*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      data->SetValue(fValueFormula->EvalInstance(iData));
   }
}
