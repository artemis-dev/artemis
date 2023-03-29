/**
 * @file   TTreeFormulaGateProcessor.cc
 * @brief  gate generator using TTreeFormula
 *
 * @date   Created       : 2014-11-04 10:52:39 JST
 *         Last Modified : 2016-07-21 11:36:33 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TTreeFormulaGateProcessor.h"

#include <TRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TTreeFormulaManager.h>
#include <TGateArray.h>
#include <TGateObject.h>

namespace {
   const char vartag = '#';
}

using art::TTreeFormulaGateProcessor;

ClassImp(art::TTreeFormulaGateProcessor)

namespace {
   const TString DEFAULT_DEFINITION("gatename; branch.fVal > 0");
}

TTreeFormulaGateProcessor::TTreeFormulaGateProcessor()
   : fTree(NULL) {
   RegisterProcessorParameter("Definitions","gate definitions",
			      fDefinitions, StringVec_t(1,DEFAULT_DEFINITION));
}

TTreeFormulaGateProcessor::~TTreeFormulaGateProcessor()
{
   for(Int_t i = 0, n = fFormulae.size(); i != n; ++i) {
      delete fFormulae[i];
   }
   delete fTree;
}

TString TTreeFormulaGateProcessor::Substitute(TString str, TGateArray *ga)
{
   if (str.CountChar(vartag) < 2) {
      return str;
   }

   Ssiz_t ext;
   const Ssiz_t index = str.Index(TRegexp(Form("\\%c[^\\$c]+\\$c",vartag,vartag,vartag)),&ext);

   const TString gatename = str(index+1,ext-2);
   if (!ga->IsDefined(gatename)) {
      SetStateError(TString::Format("gate not defined: %s",gatename.Data()));
      return str;
   }

   const Int_t gateid = ga->GetID(gatename);
   // Substitute recursively
   return Substitute(str.Replace(index,ext,TString::Format("gate.Test(%d)",gateid)),ga);
}

void TTreeFormulaGateProcessor::PrepareTree(TEventCollection *col)
{
   fTree = new TTree("hogetree","hogetree");
   fTree->SetDirectory(NULL);
   TIter *it = col->GetIter();
   while (TEventObject *const obj = static_cast<TEventObject*>(it->Next())) {
      obj->SetBranch(fTree);
//      fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
   }
   fTree->SetCircular(1);
}

void TTreeFormulaGateProcessor::Init(TEventCollection *col)
{
   TGateArray **const gateArray = reinterpret_cast<TGateArray**>(col->GetObjectRef("gate"));
   if(!gateArray) {
      SetStateError("gate array not found. Run TGateArrayInitializer before this.");
      return;
   }

   if (!fDefinitions.front().CompareTo(DEFAULT_DEFINITION)) {
      SetStateError("default input found.");
      return;
   }

   PrepareTree(col);

   const Int_t nDefinitions = fDefinitions.size();
   fFormulae.assign(nDefinitions,NULL);
   fManagers.assign(nDefinitions,NULL);
   fGates.resize(nDefinitions);
   for (Int_t i = 0; i != nDefinitions; ++i) {
      const TString definition = fDefinitions.at(i);
      const TObjArray *tokens = definition.Tokenize(";");
      if (tokens->GetEntriesFast() < 2) {
	 SetStateError(TString::Format("Invalid definition: %s",definition.Data()));
	 delete tokens;
	 return;
      }
      const TString name    = static_cast<TObjString*>(tokens->At(0))->GetString().Strip(TString::kTrailing,' ');
      const TString formula = static_cast<TObjString*>(tokens->At(1))->GetString().Strip(TString::kLeading,' ');

      delete tokens;
      tokens = NULL;

      // substitution of $gatename$ to gate.Test(gateid)
      if (formula.CountChar(vartag) % 2) {
	 SetStateError(TString::Format("Number of %c should be even in formula: %s",vartag,formula.Data()));
	 return;
      }

      const TString formula_s = Substitute(formula,*gateArray);
      if (IsError()) return; // Substitute() may fail

      fManagers[i] = new TTreeFormulaManager;
      fFormulae[i] = new TTreeFormula(name,formula_s,fTree);

      if (!fFormulae[i]->GetTree()) { // compilation of formula failed
	 SetStateError(TString::Format("Invalid formula: %s",formula_s.Data()));
	 return;
      }

      fManagers[i]->Add(fFormulae[i]);
      fManagers[i]->Sync();

      fGates[i] = (*gateArray)->New(name,formula);
      if (!fGates[i]) {
	 SetStateError(TString::Format("Failed to add gate: %s",name.Data()));
      }
   }
}

void TTreeFormulaGateProcessor::Process()
{
   fTree->GetEntry(1);
   for (Int_t i = 0, n = fDefinitions.size(); i != n; ++i) {
      for (Int_t iData = 0, nData = fManagers[i]->GetNdata(); iData != nData; ++iData) {
	 if ((Bool_t)fFormulae[i]->EvalInstance(iData)) {
	    fGates[i]->SetState(kTRUE);
	    break;
	 }
      }
   }
}
