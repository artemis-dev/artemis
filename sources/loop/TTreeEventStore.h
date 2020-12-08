/* $Id:$ */
/**
 * @file   TTreeEventStore.h
 * @date   Created : Jul 11, 2013 21:11:30 JST
 *   Last Modified : 2020-11-27 00:16:23 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TTREEEVENTSTORE_H
#define TTREEEVENTSTORE_H

#include "TProcessor.h"
#include "IEventStore.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


namespace art {
   class TTreeEventStore;
   class TConditionBit;
   class TEventHeader;
}

class TString;
class TFile;
class TTree;
class TList;

class art::TTreeEventStore  : public TProcessor, public IEventStore {

public:
   TTreeEventStore();
   ~TTreeEventStore();


   virtual void Init(TEventCollection *col);
   virtual void Process();

   Int_t GetRunNumber() const; 
   const char* GetRunName() const;
   
   
private:
   
   TString  fFileName;
   TString  fTreeName;
   TFile   *fFile; //!
   TTree   *fTree; //!
   TList   *fObjects; //!
   Long_t   fEventNum;
   Long_t   fMaxEventNum;

   TEventHeader **fEventHeader; //!

#ifdef USE_MPI   
   Int_t fRankID; //!
   Int_t fNPE; //! number of pe
#endif   
   
   
   ClassDef(TTreeEventStore,1);
};
#endif // end of #ifdef TTREEEVENTSTORE_H
