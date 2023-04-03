/**
 * @file   TSeparateOutputProcessor.cc
 * @brief  separate output into named outputs
 *
 * @date   Created       : 2014-06-17 01:59:29 JST
 *         Last Modified : Jun 21, 2014 15:23:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TSeparateOutputProcessor.h"
#include <TClonesArray.h>
#include <TClass.h>
#include <TDataObject.h>

using art::TSeparateOutputProcessor;

ClassImp(TSeparateOutputProcessor)

TSeparateOutputProcessor::TSeparateOutputProcessor()
{
   StringVec_t defval;
   RegisterInputCollection("InputCollection","name of input collection",fInputColName,TString("inputname"));
   RegisterProcessorParameter("OutputCollections","list of name of output collection",
                              fOutputColNames,defval);
                           
}

TSeparateOutputProcessor::~TSeparateOutputProcessor()
{
}

TSeparateOutputProcessor::TSeparateOutputProcessor(const TSeparateOutputProcessor& rhs)
{
}

TSeparateOutputProcessor& TSeparateOutputProcessor::operator=(const TSeparateOutputProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TSeparateOutputProcessor::Init(TEventCollection *col)
{
   
   fInput = (TClonesArray**)col->GetObjectRef(fInputColName);
   if (!fInput) {
      SetStateError(TString::Format("No such input colllection '%s'",fInputColName.Data()));
      return;
   }
   if (!(*fInput)->GetClass()->InheritsFrom(TDataObject::Class())) {
      SetStateError(TString::Format("Class '%s' in input TClonesArray '%s' is not inherits from TDataObject",
                                    (*fInput)->GetClass()->GetName(),fInputColName.Data()));
      return;
   }

   for (StringVec_t::iterator it = fOutputColNames.begin(); it != fOutputColNames.end(); it++) {
      TClonesArray *output = NULL;
      if (!it->IsNull()) {
         output = new TClonesArray((*fInput)->GetClass());
         col->Add(it->Data(),output,fOutputIsTransparent);
      }
      fOutput.push_back(output);
   }
}

void TSeparateOutputProcessor::Process()
{
   Int_t nHits = (*fInput)->GetEntriesFast();
   Int_t nOutputSize = fOutput.size();;
   for (std::vector<TClonesArray*>::iterator it = fOutput.begin(); it != fOutput.end(); it++) {
      if (*it) {
         (*it)->Clear("C");
      }
   }

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TDataObject *data = static_cast<TDataObject*>((*fInput)->At(iHit));
      const Int_t &id = data->GetID();
      if (id >= nOutputSize || fOutput[id] == NULL) continue;
      TDataObject *out = (TDataObject*) fOutput[id]->ConstructedAt(fOutput[id]->GetEntriesFast());
      data->Copy(*out);
      out->SetID(0);
      out->SetAuxID(data->GetID());
   }
}
