/**
 * @file   TCatTPCRecoilCandidateRescanByDistanceProcessor.h
 * @brief
 *
 * @date   Created       : 2016-05-20 08:59:27 JST
 *         Last Modified : 2016-05-20 14:35:52 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_67F7EC6F_3F9B_4771_AAC0_62077F7955A0
#define INCLUDE_GUARD_UUID_67F7EC6F_3F9B_4771_AAC0_62077F7955A0

#include "TCatTPCRecoilCandidateSelectProcessor.h"

namespace art {
   class TCatTPCRecoilCandidateRescanByDistanceProcessor;
}

class art::TCatTPCRecoilCandidateRescanByDistanceProcessor : public TCatTPCRecoilCandidateSelectProcessor {
public:
   TCatTPCRecoilCandidateRescanByDistanceProcessor();
   virtual ~TCatTPCRecoilCandidateRescanByDistanceProcessor();

   TCatTPCRecoilCandidateRescanByDistanceProcessor(const TCatTPCRecoilCandidateRescanByDistanceProcessor& rhs);
   TCatTPCRecoilCandidateRescanByDistanceProcessor& operator=(const TCatTPCRecoilCandidateRescanByDistanceProcessor& rhs);

   virtual void Process();


protected:

private:
   TString fInputCandidateName; // name of candidate collection
   TClonesArray **fInputCandidate; //! name of output collection
   


   ClassDef(TCatTPCRecoilCandidateRescanByDistanceProcessor,1) //
};

#endif // INCLUDE_GUARD_UUID_67F7EC6F_3F9B_4771_AAC0_62077F7955A0
