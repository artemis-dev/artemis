/**
 * @file   TMergeTrackProcessor.h
 * @brief
 *
 * @date   Created:       2023-10-17 10:33:43 JST
 *         Last Modified: 2023-10-17 11:19:32 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    Copyright (C) 2013 OTA Shinsuke All rights reserved
 */


#ifndef TMERGETRACKPROCESSOR_H
#define TMERGETRACKPROCESSOR_H 1

#include "TProcessor.h"
#include <vector>

namespace art {
   class TMergeTrackProcessor;
}

class TClonesArray;


class art::TMergeTrackProcessor : public TProcessor {
public:

   TMergeTrackProcessor();
   virtual ~TMergeTrackProcessor();

   virtual void Init(TEventCollection *col) override;
   virtual void Process() override;


protected:
   Parameter<std::vector<TString>> fInputNames; //!
   Parameter<TString> fOutputName; //!

   std::vector<TClonesArray**> fInputs; //!
   TClonesArray* fOutput; //!


   ClassDef(TMergeTrackProcessor,0) // merging TTrack 
};

#endif
