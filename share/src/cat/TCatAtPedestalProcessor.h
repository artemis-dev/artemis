/* $Id:$ */
/**
 * @file   TCatAtPedestalProcessor.h
 * @date   Created : Feb 06, 2013 14:06:28 JST
 *   Last Modified : Feb 11, 2013 14:25:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATPEDESTALPROCESSOR_H
#define TCATATPEDESTALPROCESSOR_H

#include <TProcessor.h>
#include <TH2F.h>

class TClonesArray;

namespace art {
   class TCategorizedData;
   class TCatAtPedestalProcessor;
}

class art::TCatAtPedestalProcessor  : public art::TProcessor {

public:
   TCatAtPedestalProcessor();
   ~TCatAtPedestalProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();
   virtual void EndOfRun();
   
   StringVec_t fInputColName;
   TString     fOutputColName;
   Int_t fMaxID;
   Int_t fMaxGeo;
   TCategorizedData **fCategorized;
   TClonesArray *fCATHit;
   TString fHistName;
   TH2F    *fHistPed;
   TH2F    *fHistPedGeoCh;

   ClassDef(TCatAtPedestalProcessor,1);

};
#endif // end of #ifdef TCATATPEDESTALPROCESSOR_H
