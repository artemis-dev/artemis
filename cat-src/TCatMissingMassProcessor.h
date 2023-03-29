/**
 * @file   TCatMissingMassProcessor.h
 * @brief  calculate missing mass from beam track and recoil track
 *
 * @date   Created       : 2016-07-24 22:38:47 JST
 *         Last Modified : 2017-06-28 17:31:57 JST by tokieda
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5C8DD53F_FD8A_4BCC_9A60_E2D2EE735079
#define INCLUDE_GUARD_UUID_5C8DD53F_FD8A_4BCC_9A60_E2D2EE735079

#include "TProcessor.h"

namespace art {
   class TCatMissingMassProcessor;
   class TRangeTableHelper;
}

class TClonesArray;

class art::TCatMissingMassProcessor : public TProcessor {
public:
   TCatMissingMassProcessor();
   virtual ~TCatMissingMassProcessor();

   TCatMissingMassProcessor(const TCatMissingMassProcessor& rhs);
   TCatMissingMassProcessor& operator=(const TCatMissingMassProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fNameBeamInput;
   TClonesArray **fBeamInput; //!

   TString fNameRecoilInput;
   TClonesArray **fRecoilInput; //!
   
   TString fNameRecoilOutput;
   TClonesArray *fRecoilOutput; //!
   
   TString fNameEx;
   Double_t *fEx; //!

   TString fNameThetaCM;
   Double_t *fThetaCM; //!

   TString fRangeTableName;
   TRangeTableHelper **fRangeTableHelper; //!

   // Beam (default: 132Sn, 114 MeV/u)
   Int_t fMassNumber;    // default: A = 132
   Int_t fAtomicNumber;  // default: Z = 50
   Double_t fBeamEnergy; // default: 114 MeV/u

private:

   ClassDef(TCatMissingMassProcessor,1) // calculate missing mass from beam track and recoil track
};

#endif // INCLUDE_GUARD_UUID_5C8DD53F_FD8A_4BCC_9A60_E2D2EE735079
