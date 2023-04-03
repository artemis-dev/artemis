/**
 * @file   TPIDProcessor.h
 * @brief  particle identification
 *
 * @date   Created       : 2015-04-16 01:52:09 JST
 *         Last Modified : 2018-02-04 13:15:05 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_F948CB33_3A1B_48AC_BC67_A1BCF23EDF62
#define INCLUDE_GUARD_UUID_F948CB33_3A1B_48AC_BC67_A1BCF23EDF62

#include "TProcessor.h"

namespace art {
   namespace ms1502 {
      class TPIDProcessor;
   }
}

class TClonesArray;

class art::ms1502::TPIDProcessor : public TProcessor {
public:
   TPIDProcessor();
   virtual ~TPIDProcessor();

   TPIDProcessor(const TPIDProcessor& rhs);
   TPIDProcessor& operator=(const TPIDProcessor& rhs);

   virtual void Process();

   

protected:
   TString fInputNameBrho1; // name of brho at the first section
   TString fInputNameBrho2; // name of brho at the second section
   TString fInputNameBeta1; // name of beta at the first section
   TString fInputNameBeta2; // name of beta at the second section
   TString fInputNameDF; // name of dispersive focus
   TClonesArray **fInputBrho1; //! input collection of brho at the first section 
   TClonesArray **fInputBrho2; //! input collection of brho at the second section
   TClonesArray **fInputBeta1; //! input collection of beta at the first section 
   TClonesArray **fInputBeta2; //! input collection of beta at the second section
   TClonesArray **fInputDF; //! input collection of dispersive focus

   FloatVec_t fDegraderThickness; // d = p0 + p1 * x
   Float_t fAtomicNumberNormalization; // normalization factor to calculate atomic number

   TString fOutputNameAQ35;
   TString fOutputNameAQ57;
   TString fOutputNameZdeg;
   
   TClonesArray *fOutputAQ35; //!
   TClonesArray *fOutputAQ57; //!
   TClonesArray *fOutputZdeg; //!




private:

   ClassDef(TPIDProcessor,1) // particle identification
};

#endif // INCLUDE_GUARD_UUID_F948CB33_3A1B_48AC_BC67_A1BCF23EDF62
