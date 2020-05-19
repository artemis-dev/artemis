/* @file TChargeValidator.cc
 * @brief charge validation
 *
 * @date Create        : 2019-06-07 10:51:11 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TChargeValidator.h"
#include "ICharge.h"

ClassImp(art::TChargeValidator)

using art::TChargeValidator;

TChargeValidator::TChargeValidator()
{
   fInput.SetDoAuto(true);
   fOutput.SetDoAuto(false);
   Register(fInput("Input","name of input",TString("inputCharge")));
   Register(fOutput("Output","name of output",TString("outputCharge")));
   Register(fChargeRange("ChargeRange","range of chage [min,max]",DoubleVec_t(2,0.)));
}

TChargeValidator::~TChargeValidator()
{
}

void TChargeValidator::Init(TEventCollection *col)
{
   DoubleVec_t& range = fChargeRange.Value();
   if (range.size() != fChargeRange.DefaultValue().size()) {
      SetStateError(Form("%s should have %zu parameters",fChargeRange.GetName(),fChargeRange.DefaultValue().size()));
      return;
   }
   if (range[0] > range[1]) {
      std::swap(range[0],range[1]);
      Info("Init","%s has been swapped",fChargeRange.GetName());
   }

   fOutput.SetData(new TClonesArray(fInput->GetClass()));
   col->Add(fOutput.Value(),fOutput.fData,fOutputIsTransparent);
}

void TChargeValidator::Process()
{
   fOutput->Clear("C");
   for (int i = 0, n = fInput->GetEntriesFast(); i < n; ++i) {
      TObject *input = fInput->UncheckedAt(i);
      Double_t charge =dynamic_cast<ICharge*>(input)->GetCharge();
      const DoubleVec_t& range = fChargeRange.Value();
      if (charge < range[0] || range[1] < charge) continue;
      TObject *output = fOutput->ConstructedAt(fOutput->GetEntriesFast());
      input->Copy(*output);
   }
}
