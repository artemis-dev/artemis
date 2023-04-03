/**
 * @file   TBrhoReconstructProcessor.h
 * @brief  reconstruct Brho35, Brho57 using F3, F5, F7 information
 *
 * @date   Created       : 2015-04-15 14:55:13 JST
 *         Last Modified : 2018-02-04 13:14:37 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_F18756D7_A443_440A_8285_6CD7C5C849B7
#define INCLUDE_GUARD_UUID_F18756D7_A443_440A_8285_6CD7C5C849B7

#include "TProcessor.h"
#include <TMatrixD.h>

namespace art {
   class TBrhoReconstructProcessor;
}

class TClonesArray;

class art::TBrhoReconstructProcessor : public TProcessor {
public:
   TBrhoReconstructProcessor();
   virtual ~TBrhoReconstructProcessor();

   TBrhoReconstructProcessor(const TBrhoReconstructProcessor& rhs);
   TBrhoReconstructProcessor& operator=(const TBrhoReconstructProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputName1; // name of tracking result at entrance focus
   TString fInputName2; // name of tracking result at exit focus
   TClonesArray **fInput1; //! input collection of tracking result at entrance focus
   TClonesArray **fInput2; //! input collection of tracking result at exit focus

   TString fOutputName; // name of output collection (brho)
   TClonesArray *fOutput; //! output collection of brho
#if 0 // currently not used
   TMatrixD **fGlobalMatrix1; //! global optical matrix element at entrance focus (LISE)
   TMatrixD **fGlobalMatrix2; //! global optical matrix element at exit focus (LISE)
#endif // #if 0

   TMatrixD *fMatrix; //! optical matrix element from entrance to exit

   Float_t fBrho0; // central Brho

   Float_t fZ; // z position of focus of exit (mode 1) or entrance (mode 2)

   Int_t fMode; // 0: both tracks on focus, 1: only entrance track on focus, 2: only exit track on focus

   Int_t fSection; // section 35: F3-F5, 57: F5-F7


private:

   ClassDef(TBrhoReconstructProcessor,1) // reconstruct Brho35, Brho57 using F3, F5, F7 information
};

#endif // INCLUDE_GUARD_UUID_F18756D7_A443_440A_8285_6CD7C5C849B7
