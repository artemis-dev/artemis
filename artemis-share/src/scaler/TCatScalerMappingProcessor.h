/* $Id:$ */
/**
 * @file   TCatScalerMappingProcessor.h
 * @date   Created : Feb 13, 2013 20:13:33 JST
 *   Last Modified : Oct 31, 2013 06:50:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATSCALERMAPPINGPROCESSOR_H
#define TCATSCALERMAPPINGPROCESSOR_H

#include <TProcessor.h>

#include "TCatScalerData.h"

namespace art {
  class TCategorizedData;
  class TCatScalerMappingProcessor;
}

class art::TCatScalerMappingProcessor  : public art::TProcessor {

public:
   TCatScalerMappingProcessor();
   ~TCatScalerMappingProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void BeginOfRun();
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString fOutputColNameTotal;
   TString fOutputColNameDiff;
   TCategorizedData **fCategorized;   
   Int_t fCatID;
   Int_t fNumChannel;
   TCatScalerData *fScalerTotal;
   TCatScalerData *fScalerDiff; 
   TCatScalerData *fScalerSaved; 

   ClassDef(TCatScalerMappingProcessor,1);

};
#endif // end of #ifdef TCATSCALERMAPPINGPROCESSOR_H
