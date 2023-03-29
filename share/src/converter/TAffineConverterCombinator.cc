/**
 * @file   TAffineConverterCombinator.cc
 * @brief  generate combined affine converter
 *
 * @date   Created       : 2017-08-04 08:47:13 JST
 *         Last Modified : 2017-08-04 09:58:56 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TAffineConverterCombinator.h"
#include <TAffineConverter.h>
#include <TClonesArray.h>

using art::TAffineConverterCombinator;

ClassImp(TAffineConverterCombinator)

TAffineConverterCombinator::TAffineConverterCombinator()
{
   RegisterProcessorParameter("NameOutput","name of output information",fNameOutput,TString("conv"));
}

TAffineConverterCombinator::~TAffineConverterCombinator()
{
}

TAffineConverterCombinator::TAffineConverterCombinator(const TAffineConverterCombinator& rhs)
{
}

TAffineConverterCombinator& TAffineConverterCombinator::operator=(const TAffineConverterCombinator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TAffineConverterCombinator::Init(TEventCollection *col)
{
   if (col->GetInfo(fNameOutput)) {
      SetStateError(Form("UserInfo named '%s' already exists",fNameOutput.Data()));
      return;
   }

   TAffineConverterArrayGenerator::Init(col);

   if (fConverterArrayList->GetEntries() == 0) {
      SetStateError("No converter is ready");
      return;
   }

   TClonesArray *converterArray = new TClonesArray(TAffineConverter::Class(),100);
   TClonesArray *convpre = static_cast<TClonesArray*>(fConverterArrayList->First());
   const Int_t n = convpre->GetEntriesFast();
   for (Int_t i = 0; i < n; ++i) {
      art::TAffineConverter *const conv = 
	 static_cast<art::TAffineConverter*>(converterArray->ConstructedAt(i));
      conv->Set(0,1.);
   }

   TIter next(fConverterArrayList);
   while (TObject *obj = next()) {
      TClonesArray *convarray = static_cast<TClonesArray*>(obj);
      for (Int_t i = 0; i < n; ++i) {
         art::TAffineConverter *const conv = 
            static_cast<art::TAffineConverter*>(converterArray->At(i));
         art::TAffineConverter *const conv1 = 
            static_cast<art::TAffineConverter*>(convarray->At(i));
         Double_t a0 = 0., b0 = 1.;
         Double_t a1 = 0., b1 = 1.;
         conv->Get(a0,b0);
         conv1->Get(a1,b1);
         a0 = a1 + b1 * a0;
         b0 = b1 * b0;
         conv->Set(a0,b0);
      }
   }
   fConverterArrayList->Add(converterArray);
   col->AddInfo(fNameOutput,converterArray,fOutputIsTransparent);
}
