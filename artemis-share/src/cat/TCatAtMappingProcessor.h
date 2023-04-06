/* $Id:$ */
/**
 * @file   TCatAtMappingProcessor.h
 * @date   Created : Feb 06, 2013 14:06:28 JST
 *   Last Modified : 2017-12-24 17:49:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATMAPPINGPROCESSOR_H
#define TCATATMAPPINGPROCESSOR_H

#include <TProcessor.h>

#include <TCatReadoutPadArray.h>

class TClonesArray;


namespace art {
   class TCategorizedData;
   class TCatAtMappingProcessor;
   class TCatReadoutPadArray;
}

class art::TCatAtMappingProcessor  : public art::TProcessor {

public:
   TCatAtMappingProcessor();
   ~TCatAtMappingProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString     fOutputColName;
   TCategorizedData **fCategorized; //!
   TClonesArray        *fCATHit;
   Int_t       fCatID;
   TCatReadoutPadArray *fPads; //!
   TString fReadoutPadName;


   FloatVec_t fBeamRegion; // x coordinate of beam region: default (-18,18)
   Int_t fSelectIf; // select recoil if 1 or beam if 0
   
   Int_t fMaxDetID; // max id of detector: default 415 (for type 2 readout pad)

   ClassDef(TCatAtMappingProcessor,1)
};
#endif // end of #ifdef TCATATMAPPINGPROCESSOR_H
