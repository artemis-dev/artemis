/**
 * @file   TCatRefineRecoilClusterProcessor.h
 * @brief  refine recoil cluster
 *
 * @date   Created       : 2015-05-14 13:56:09 JST
 *         Last Modified : 2018-02-04 13:13:45 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_BDDEBD89_E749_464E_98EF_E6FE53132019
#define INCLUDE_GUARD_UUID_BDDEBD89_E749_464E_98EF_E6FE53132019

#include "TProcessor.h"

namespace art {
   class TCatRefineRecoilClusterProcessor;
   class TSimpleData;
}

class TClonesArray;

class art::TCatRefineRecoilClusterProcessor : public TProcessor {
public:
   TCatRefineRecoilClusterProcessor();
   virtual ~TCatRefineRecoilClusterProcessor();

   TCatRefineRecoilClusterProcessor(const TCatRefineRecoilClusterProcessor& rhs);
   TCatRefineRecoilClusterProcessor& operator=(const TCatRefineRecoilClusterProcessor& rhs);
   virtual void Process();

protected:
   TString fInputName; // input collection name
   TClonesArray** fInput; //! input array

   TString fOutputNameDistance; //
   TClonesArray* fDistance; //!

   TString fOutputNameLength; // name of output :path length in X-Y plane
   TSimpleData* fLength; //! name of output :path length in X-Y plane
   
   TString fOutputName; // output collection name
   TClonesArray* fOutput; //! output array

   Int_t fRequredNumHits; // the number of hits resured to continue analysis
   
private:

   ClassDef(TCatRefineRecoilClusterProcessor,1) // refine recoil cluster
};

#endif // INCLUDE_GUARD_UUID_BDDEBD89_E749_464E_98EF_E6FE53132019
