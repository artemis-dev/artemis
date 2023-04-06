/* $Id:$ */
/**
 * @file   TCatAtHitCandidateProcessor.h
 * @date   Created : Oct 26, 2013 02:26:50 JST
 *   Last Modified : Oct 26, 2013 04:28:38 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATHITCANDIDATEPROCESSOR_H
#define TCATATHITCANDIDATEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCatAtHitCandidateProcessor;
}

class art::TCatAtHitCandidateProcessor  : public art::TProcessor {

public:
   TCatAtHitCandidateProcessor();
   ~TCatAtHitCandidateProcessor();
   virtual Init(TEventCollection *col);
   virtual Process();

private:
   TClonesArray **fCatHits; // input array
   TClonesArray  *fPosition; // output array

   StringVec_t fInputColName; // input collection name
   TString     fOutputColName; // output collection name

};
#endif // end of #ifdef TCATATHITCANDIDATEPROCESSOR_H
