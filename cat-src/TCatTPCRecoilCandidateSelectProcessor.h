/**
 * @file   TCatTPCRecoilCandidateSelectProcessor.h
 * @brief  select recoil candidate
 *
 * @date   Created       : 2016-04-17 04:45:26 JST
 *         Last Modified : 2018-02-04 13:15:41 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD
#define INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD

#include "TProcessor.h"

namespace art {
   class TCatTPCRecoilCandidateSelectProcessor;
   class TCatReadoutPadArray;
}

class TVector3;
class TClonesArray;

class art::TCatTPCRecoilCandidateSelectProcessor : public TProcessor {
public:
   TCatTPCRecoilCandidateSelectProcessor();
   virtual ~TCatTPCRecoilCandidateSelectProcessor();

   TCatTPCRecoilCandidateSelectProcessor(const TCatTPCRecoilCandidateSelectProcessor& rhs);
   TCatTPCRecoilCandidateSelectProcessor& operator=(const TCatTPCRecoilCandidateSelectProcessor& rhs);

   virtual void Process();

   virtual void Init(TEventCollection *col);

protected:

   TString               fInputName;      // name of input collection
   TClonesArray        **fInput;          //! input array
   TString               fOutputName;     // name of output collection
   TClonesArray         *fOutput;         //! output array
   TString               fReadoutPadArrayName; // name of readout pad array 
   TCatReadoutPadArray **fReadoutPadArray;     //! readout pad array

   TString               fNameInertia; // name of output moment of inertia
   Double_t             *fInertia; //!
   TString               fNamePrincipalAxis; // name of output principal axis
   TVector3             *fPrincipalAxis;  //!
   TString               fNameDeviation; // deviation
   TVector3             *fDeviation; //!

   FloatVec_t fBeamRegion; // x coordinate of beam region: default (-18,18)

   FloatVec_t fClusterRadius; // size of cluster (mm)

   Float_t fMinClusterSize; // required minimum cluster size


private:
   

   ClassDef(TCatTPCRecoilCandidateSelectProcessor,1) // select recoil candidate
};

#endif // INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD
