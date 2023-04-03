/* $Id:$ */
/**
 * @file   TCatSumSampleProcessor.h
 * @date   Created : Mar 11, 2013 05:11:23 JST
 *   Last Modified : Oct 25, 2013 20:59:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATSUMSAMPLEPROCESSOR_H
#define TCATSUMSAMPLEPROCESSOR_H

#include <TProcessor.h>

class TClonesArray;
namespace art {
   class TCatSumSampleProcessor;
}

class art::TCatSumSampleProcessor  : public art::TProcessor {

public:
   TCatSumSampleProcessor();
   ~TCatSumSampleProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   

protected:
   TClonesArray **fCatHit; //!
   TClonesArray *fCatHitOut;

   StringVec_t  fInputColName;
   TString      fOutputColName;
   Int_t        fIsSparse; 
   Int_t        fOverwrite;

   ClassDef(TCatSumSampleProcessor,1);
};
#endif // end of #ifdef TCATSUMSAMPLEPROCESSOR_H
