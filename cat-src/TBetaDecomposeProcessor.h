/**
 * @file   TBetaDecomposeProcessor.h
 * @brief  Decompose TOF to beta1 and beta2 using brho1 and brho2
 *
 * @date   Created       : 2015-04-14 08:12:18 JST
 *         Last Modified : 2018-02-04 13:14:07 JST (ota)
 * @author  <ota@cns.s.u-tokyo.acjp>
 *
 *    (C) 2015
 */

#ifndef INCLUDE_GUARD_UUID_4355FEDB_A63E_488E_B5E6_D155A171AD06
#define INCLUDE_GUARD_UUID_4355FEDB_A63E_488E_B5E6_D155A171AD06

#include "TProcessor.h"

namespace art {
   class TBetaDecomposeProcessor;
}

class TClonesArray;

class art::TBetaDecomposeProcessor : public TProcessor {
public:
   TBetaDecomposeProcessor();
   virtual ~TBetaDecomposeProcessor();

   TBetaDecomposeProcessor(const TBetaDecomposeProcessor& rhs);
   TBetaDecomposeProcessor& operator=(const TBetaDecomposeProcessor& rhs);

   void Process();


protected:
   TString fInputNameBrho1; // name of input collection for Brho of the first section
   TString fInputNameBrho2; // name of input collection for Brho of the second section
   TString fInputNameTOF; // name of input collection for TOF
   TClonesArray **fInputBrho1; //! input collection of Brho of the first section (TSimpleData)
   TClonesArray **fInputBrho2; //! input collection of Brho of the second section (TSimpleData)
   TClonesArray **fInputTOF; //! input collection of total TOF through both of the first and second section (TTimeDifference)

   TString fOutputNameBeta1; // name of output collection for beta of the first section
   TString fOutputNameBeta2; // name of output collection for beta of the second section
   TClonesArray *fOutputBeta1; //! output collection of beta of the first section
   TClonesArray *fOutputBeta2; //! output collection of beta of the second section

   Float_t fLength1; // length of the first section
   Float_t fLength2; // length of the first section



private:

   ClassDef(TBetaDecomposeProcessor,1) // Decompose TOF to beta1 and beta2 using brho1 and brho2
};

#endif // INCLUDE_GUARD_UUID_4355FEDB_A63E_488E_B5E6_D155A171AD06
