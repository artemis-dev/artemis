/**
 * @file   TScalerMappingProcessor.h
 * @brief  event-by-event scaler mapping processor
 *
 * @date   Created       : 2018-06-27 17:37:47 JST
 *         Last Modified : 2018-06-27 17:39:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_F58FDFD9_56CF_4383_919D_682CFFB87E43
#define INCLUDE_GUARD_UUID_F58FDFD9_56CF_4383_919D_682CFFB87E43

#include "TProcessor.h"

namespace art {
   class TScalerMappingProcessor;
   class TCategorizedData;
   class TScalerData;

}

class art::TScalerMappingProcessor : public TProcessor {
public:
   TScalerMappingProcessor();
   virtual ~TScalerMappingProcessor();

   TScalerMappingProcessor(const TScalerMappingProcessor& rhs);
   TScalerMappingProcessor& operator=(const TScalerMappingProcessor& rhs);

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
   TScalerData *fScalerTotal;
   TScalerData *fScalerDiff; 
   TScalerData *fScalerSaved; 

private:

   ClassDef(TScalerMappingProcessor,1) // event-by-event scaler mapping processor
};

#endif // INCLUDE_GUARD_UUID_F58FDFD9_56CF_4383_919D_682CFFB87E43
