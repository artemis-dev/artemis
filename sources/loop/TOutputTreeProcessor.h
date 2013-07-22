/* $Id:$ */
/**
 * @file   TOutputTreeProcessor.h
 * @date   Created : Jul 11, 2013 17:11:09 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TOUTPUTTREEPROCESSOR_H
#define TOUTPUTTREEPROCESSOR_H

#include <TProcessor.h>
#include <TFile.h>
#include <TTree.h>

namespace art {
   class TOutputTreeProcessor;
}

class art::TOutputTreeProcessor  : public TProcessor {

public:
   TOutputTreeProcessor();
   virtual ~TOutputTreeProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void PreLoop();
   virtual void PostLoop();
   virtual void EndOfRun();
   

private:
   TString  fFileName;  // output file name
   TString  fTreeName;  // output tree name
   TFile   *fFile;
   TTree   *fTree;      //-> output tree
   TList   *fObjects;

   ClassDef(TOutputTreeProcessor,1);
};
#endif // end of #ifdef TOUTPUTTREEPROCESSOR_H
