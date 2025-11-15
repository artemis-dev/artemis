/**
 * @file   TPPACAnodeTimingProcessor.cc
 * @brief  extract timing from multiple PPAC anodes
 *
 * @date   Created       : 2016-05-25 13:01:14 JST
 *         Last Modified : 2016-06-01 08:14:10 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TPPACAnodeTimingProcessor.h"

#include <iostream>
#include <iterator>
#include <algorithm>

#include <TClonesArray.h>
#include <TTimingData.h>
#include <TPPACData.h>
#include <TConfigFile.h>

using art::TPPACAnodeTimingProcessor;

ClassImp(TPPACAnodeTimingProcessor)

TPPACAnodeTimingProcessor::TPPACAnodeTimingProcessor()
  : fInput(NULL)
{
   StringVec_t defInput;
   defInput.push_back("f7ppac1a");
   defInput.push_back("f7ppac1b");
   defInput.push_back("f7ppac2a");
   defInput.push_back("f7ppac2b");
   RegisterInputCollection("Input","input ppacs",
			   fInputColName, defInput);
   RegisterProcessorParameter("ParameterFile","timing offset parameter file",
			      fParameterFile,TString("path/to/file"));
   RegisterProcessorParameter("UseTSumGate",
			      "reject anode timings which don't fulfill TSum conditions",
			      fUseTSumGate,kTRUE);
   RegisterOutputCollection("Output","output TTimingData",
			    fOutputName,TString("f7ppac_t"),&fOutput,
			    TClonesArray::Class_Name(),output_t::Class_Name());
}

TPPACAnodeTimingProcessor::~TPPACAnodeTimingProcessor()
{
   delete fInput;
}

void TPPACAnodeTimingProcessor::Init(TEventCollection *col)
{
   fInput = new std::vector<TClonesArray**>;

   /* load ppac branches */
   for (StringVec_t::const_iterator it = fInputColName.begin(),
	   end = fInputColName.end(); it != end; ++it) {

      TClonesArray **const ref
	 = reinterpret_cast<TClonesArray**>(col->GetObjectRef(it->Data()));

      if(!ref) {
	 SetStateError(TString::Format(ErrMsgFmt::INVALID_INPUT_COLLECTION,
				       it->Data()));
	 return;
      }

      if (!(*ref)->GetClass()->InheritsFrom(input_t::Class())) {
	 SetStateError(TString::Format("'%s' is not a TClonesArray of %s",
				       it->Data(), input_t::Class_Name()));
	 return;
      }

      fInput->push_back(ref);
   }

   {/* load parameter file */
      Info("Init","parameter file = %s",fParameterFile.Data());
      TConfigFile prmFile(fParameterFile,"#",", \t","#");
      for (Int_t i = 0, n = fInput->size(); i != n; ++i) {
	 const TString &token = prmFile.GetNextToken();
	 if (!token.IsFloat()) {
	    SetStateError(TString::Format("invalid parameter value: '%s'",
					  token.Data()));
	    return;
	 }
	 fTimingOffset.push_back(token.Atof());
      }

      // print loaded parameter values
      std::copy(fTimingOffset.begin(), fTimingOffset.end(),
		std::ostream_iterator<Float_t>(std::cout, " "));
      printf("\n");
   }
}

void TPPACAnodeTimingProcessor::Process()
{
   fOutput->Clear("C");

   Int_t    nValid = 0;  // number of valid anode timings
   Double_t totalT = 0.; // accumulated valid timing

   /* accumulate valid anode timings */
   for (Int_t i = 0, n = fInput->size(); i != n; ++i) {
      const TClonesArray *const carray = *fInput->at(i);
      if (carray->GetEntriesFast() == 0) {
	 continue;
      }
      const input_t *const data = static_cast<input_t*>(carray->At(0));

      if (fUseTSumGate && !data->IsValid()) {
	 // reject invalid data
	 continue;
      }

      const Double_t t = data->GetTiming();
      if (TMath::Finite(t)) {
	 ++nValid;
	 totalT += t + fTimingOffset[i];
      }
   }

   if (!nValid) { // no valid timings
      return;
   }

   {
      output_t *const out = static_cast<output_t*>(fOutput->ConstructedAt(0));
      const Double_t tave = totalT / nValid;
      out->SetTiming(tave);
      out->SetID(0);
   }
}
