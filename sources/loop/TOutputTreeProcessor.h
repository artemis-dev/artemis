/* $Id:$ */
/**
 * @file   TOutputTreeProcessor.h
 * @date   Created : Jul 11, 2013 17:11:09 JST
 *   Last Modified : 2021-07-17 20:24:58 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TOUTPUTTREEPROCESSOR_H
#define TOUTPUTTREEPROCESSOR_H

#include "TProcessor.h"
#include <TFile.h>


namespace art {
   class TOutputTreeProcessor;
   class TArtTree;
   
}

class art::TOutputTreeProcessor  : public TProcessor {

public:
   TOutputTreeProcessor();
   virtual ~TOutputTreeProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void PreLoop();
   virtual void PostLoop();

   

private:
   TString  fFileName;  // output file name
   TString  fTreeName;  // output tree name
   TFile   *fFile; //! output file
   TArtTree   *fTree; //! output tree
   TList   *fObjects; //! list of objects
   Parameter<Int_t> fSplitLevel; // split level of tree
   

   ClassDef(TOutputTreeProcessor,1);
};
#endif // end of #ifdef TOUTPUTTREEPROCESSOR_H
