/* $Id:$ */
/**
 * @file   TTreeEventStore.h
 * @date   Created : Jul 11, 2013 21:11:30 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TTREEEVENTSTORE_H
#define TTREEEVENTSTORE_H

#include <TProcessor.h>

namespace art {
   class TTreeEventStore;
   class TConditionBit;
}

class TString;
class TFile;
class TTree;
class TList;

class art::TTreeEventStore  : public TProcessor {

public:
   TTreeEventStore();
   ~TTreeEventStore();


   virtual void Init(TEventCollection *col);
   virtual void Process();
   
private:
   
   TString  fFileName;
   TString  fTreeName;
   TFile   *fFile;
   TTree   *fTree;
   TList   *fObjects;
   Long_t   fEventNum;
   Long_t   fMaxEventNum;
   ClassDef(TTreeEventStore,1);
};
#endif // end of #ifdef TTREEEVENTSTORE_H
