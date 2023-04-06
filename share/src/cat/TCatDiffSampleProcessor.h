/* $Id:$ */
/**
 * @file   TCatDiffSampleProcessor.h
 * @date   Created : Mar 11, 2013 05:11:23 JST
 *   Last Modified : 2018-02-04 12:07:28 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATDIFFSAMPLEPROCESSOR_H
#define TCATDIFFSAMPLEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCatDiffSampleProcessor;
}

class TClonesArray;

class art::TCatDiffSampleProcessor  : public TProcessor {

public:
   TCatDiffSampleProcessor();
   ~TCatDiffSampleProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   

protected:
   TClonesArray **fCatHit; //!
   TClonesArray *fCatHitOut; //!

   StringVec_t  fInputColName;
   TString      fOutputColName;
   Int_t        fIsSparse;

   ClassDef(TCatDiffSampleProcessor,1);
};
#endif // end of #ifdef TCATDIFFSAMPLEPROCESSOR_H
