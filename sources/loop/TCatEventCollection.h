/* $Id:$ */
/**
 * @file   TCatEventCollection.h
 * @date   Created : Apr 26, 2012 23:26:19 JST
 *   Last Modified : May 18, 2012 08:58:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTCOLLECTION_H
#define TCATEVENTCOLLECTION_H

#include <TTree.h>
#include <TFile.h>

class TCatEventCollection  {

public:
   static const TString kDefaultOutputName;

   TCatEventCollection();

   virtual ~TCatEventCollection();

   virtual void     Add(TNamed* obj, Bool_t isPassive = kTRUE);
   virtual void     Add(TList* list, Bool_t isPassive = kTRUE);
   virtual TObject* Get(const char* name);
   virtual void     Init();

   virtual void CreateOutput(const char *name, const char* title,
                             const char *filename = "",
                             const char *opt = "CREATE");
   virtual void Fill() { if (fOutputTree) fOutputTree->Fill(); }

   virtual void cd() { fOutputFile->cd(); }

protected:
   TList *fObjects;
   TList *fOutputObjects;
   TTree *fOutputTree;
   TFile *fOutputFile;
   TString fOutputFileName;
};


#endif // end of #ifdef TCATEVENTCOLLECTION_H
