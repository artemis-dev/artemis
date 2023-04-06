/* $Id:$ */
/**
 * @file   TCatSimpleCutProcessor.cc
 * @date   Created : Sep 27, 2012 22:27:20 JST
 *   Last Modified : Oct 01, 2012 14:44:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSimpleCutProcessor.h"

#include "TCatParameterFile.h"
#include <TCatSimpleData.h>

TCatSimpleCutProcessor::TCatSimpleCutProcessor()
{
   StringVec_t defInput; defInput.push_back("rawsignal");
   RegisterInputCollection("InputCollection","SimpleDetector in TClonesArray",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("truesignal"));
   RegisterProcessorParameter("ParameterFileName","Name of parameter file",fPrmFileName,TString("path/to/file"));

}
TCatSimpleCutProcessor::~TCatSimpleCutProcessor()
{
}

void TCatSimpleCutProcessor::Init(TCatEventCollection *col)
{
   printf("Init\n");
   fInputArray = dynamic_cast<TClonesArray*>(col->Get(fInputColName.front()));
   if (!fInputArray) {
      cout << "Error: " << fInputColName.front() << endl;
      throw 0;
   }
   TClass *cls = fInputArray->GetClass();
   if (!cls || !cls->InheritsFrom(GetInputClass())) {
      cout << "Error: " << GetInputClass() << endl;
      throw 1;
   }
   fOutputArray = new TClonesArray(fInputArray->GetClass());
   cout << cls->GetName() << endl;
   fOutputArray->SetName(fOutputColName);
   col->Add(fOutputArray,fOutputIsTransparent);
   
   TCatParameterFile prmfile(fPrmFileName);
   cout << fPrmFileName << endl;
   while (1) {
      Double_t min,max;
      if (!prmfile.GetNext(min) || !prmfile.GetNext(max)) return;
      fGateMin.push_back(min);
      fGateMax.push_back(max);
   }
   cout << "done" << endl;
}

void TCatSimpleCutProcessor::Process()
{
   fOutputArray->Clear("C");
   Int_t n= fInputArray->GetEntriesFast();
//   printf("nHits = %d\n",n);
   for (Int_t i=0; i<n; i++) {
      TCatSimpleData &data = *(TCatSimpleData*)fInputArray->At(i);
      if (IsValid(data)) {
         TObject *outdata = fOutputArray->ConstructedAt(fOutputArray->GetEntriesFast());
         data.Copy(*outdata);
      } else {
         continue;
      }
   }
}

