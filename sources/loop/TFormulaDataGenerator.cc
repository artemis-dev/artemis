/**
 * @file   TFormulaDataGenerator.cc
 * @brief  generate object from formula
 *
 * @date   Created       : 2019-03-04 01:06:48 JST
 *         Last Modified :2019-03-04 22:25:43 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shinsuke OTA
 */

#include "TFormulaDataGenerator.h"
#include "TTreeFormula.h"
#include "TTreeFormulaManager.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TSimpleData.h"
#include "TFormulaData.h"


using art::TFormulaDataGenerator;

ClassImp(TFormulaDataGenerator)

const Int_t TFormulaDataGenerator::kNumTokenInDef = 4; 

TFormulaDataGenerator::TFormulaDataGenerator()
{
   RegisterProcessorParameter("Definitions",
                              "object definitions, [name, value formula, selection formula, transparency], "
                              "transparency 0: object will filled in the tree, 1: object will NOT be filled, 2: object is not created but alias is created",
                              fDefinitions,StringVec_t());
}

TFormulaDataGenerator::~TFormulaDataGenerator()
{
}

TFormulaDataGenerator::TFormulaDataGenerator(const TFormulaDataGenerator& rhs)
{
}

TFormulaDataGenerator& TFormulaDataGenerator::operator=(const TFormulaDataGenerator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TFormulaDataGenerator::Init(TEventCollection *col)
{
   // initizalization status
   Bool_t isGood = kTRUE;
   // check the number of definitions
   Int_t numTokens = fDefinitions.size();
   if ((numTokens % kNumTokenInDef)) {
      SetStateError(Form("One definition requires four arguments : total tokens = %d",numTokens));
      return;
   }
   fNumDefs = numTokens / kNumTokenInDef;
   // check the object duplication
   TObjArray defnames;
   std::vector<TString> duplicated;
   for (Int_t i = 0; i < fNumDefs; ++i) {
      TString name = fDefinitions[kNumTokenInDef * i];
      if (!defnames.FindObject(name)) {
         defnames.Add(new TNamed(name,name));
      } else {
         duplicated.push_back(name);
      }
   }
   if (duplicated.size()) {
      TString dupnames = duplicated[0];
      for (Int_t i = 1, n = duplicated.size(); i < n; ++i) {
         dupnames.Append(", ").Append(duplicated[i]);
      }
      SetStateError(Form("duplicated objects exist: %s", dupnames.Data()));
      return;
   }
   
   Info("Init","Tree being prepared");
   // prepare default tree from event collection
   fTree = new TTree(Form("tree_%s",GetName()),"tree for variables");
   fTree->SetDirectory(NULL);
   TIter *iter = col->GetIter();
   TEventObject *obj;
   while ((obj = (TEventObject*)iter->Next())) {
      if (obj->IsObject()) {
         fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
      } else {
         TString leaflist = obj->GetName();
         if (obj->GetLength() != "") {
            leaflist.Append(TString::Format("[%s]",obj->GetLength().Data()));
         }
         leaflist.Append(TString::Format("/%s",obj->GetType()));
         fTree->Branch(obj->GetName(),*obj->GetObjectRef(),leaflist.Data());
      }
   }
   fTree->SetCircular(1);
   Info("Init","Tree has been prepared");


   // prepare definitions
   Info("Init","Preparing formula data");
   fFormulaDatas.resize(fNumDefs);
   fOutputs.resize(fNumDefs);
   for (Int_t i = 0; i < fNumDefs; ++i) {
      Int_t offset = kNumTokenInDef * i;
      const TString& name = fDefinitions[offset];
      const TString& valueFormula = fDefinitions[offset + 1];
      const TString& selectFormula = fDefinitions[offset + 2];
      Int_t transparency = fDefinitions[offset + 3].Atoi();
      if (transparency == 2) {
         fTree->SetAlias(name,valueFormula);
      } else {
         fFormulaDatas[i] = new TFormulaData;
         TClonesArray* output = fFormulaDatas[i]->CreateOutput(name, valueFormula, selectFormula, fTree);
         output->SetName(name);
         fOutputs[i] = output;
         printf("output class: %s\n",output->GetClass()->GetName());
         col->Add(name, output, transparency);
         TEventObject *obj = col->Get(name);
         fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
      }
   }
   Info("Init","Prepared formula data");
   
   for (Int_t i = 0; i < fNumDefs; ++i) {
      if (!fFormulaDatas[i]->Sync()) {
         isGood = kFALSE;
      }
   }

   if (!isGood) {
      SetStateError("Error occurs during synchronization. See warning above.");
      return;
   }

      



}

void TFormulaDataGenerator::Process()
{
   for (Int_t i = 0; i < fNumDefs; ++i) {
      fOutputs[i]->Clear("C");
   }
   fTree->GetEntry(1);
   for (Int_t i = 0; i < fNumDefs; ++i) {
      fFormulaDatas[i]->Process();
   }

}
