/**
 * @file   TMissingMassProcessor.h
 * @brief  calculate missing mass in two body reaction
 *
 * @date   Created       : 2017-09-28 15:53:13 JST
 *         Last Modified : 2018-02-15 16:32:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_CF7840FE_6BE4_47C9_BA9E_A2C417DCD7A4
#define INCLUDE_GUARD_UUID_CF7840FE_6BE4_47C9_BA9E_A2C417DCD7A4

#include "TProcessor.h"

namespace art {
   class TMissingMassProcessor;
}

class TClonesArray;

class art::TMissingMassProcessor : public TProcessor {
public:
   TMissingMassProcessor();
   virtual ~TMissingMassProcessor();

   TMissingMassProcessor(const TMissingMassProcessor& rhs);
   TMissingMassProcessor& operator=(const TMissingMassProcessor& rhs);

   virtual void Process();

protected:
   TString fNameP1; // name of particle 1 (beam)
   TString fNameP2; // name of particle 2 (target)
   TString fNameP3; // name of particle 3 (recoil)
   TString fNameP4; // name of particle 3 (recoil)
   TString fNameP4CM; // name of particle 3 (recoil)

   TClonesArray **fP1; //! particle 1
   TClonesArray **fP2; //! particle 2
   TClonesArray **fP3; //! particle 3
   TClonesArray *fP4; //! particle 4
   TClonesArray *fP4CM; //! particle 4 in cm frame
   

private:

   ClassDef(TMissingMassProcessor,1) // calculate missing mass in two body reaction
};

#endif // INCLUDE_GUARD_UUID_CF7840FE_6BE4_47C9_BA9E_A2C417DCD7A4
