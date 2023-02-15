/**
 * @file   TRawDataMappingProcessor.h
 * @brief  Mapping rawdata 
 *
 * @date   Created       : 2023-02-13 10:45:49 JST
 *         Last Modified : 2023-02-13 16:31:03 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#ifndef TRAWDATAMAPPINGPROCESSOR_H
#define TRAWDATAMAPPINGPROCESSOR_H

#include "TProcessor.h"

namespace art {
   class TRawDataMappingProcessor;
   class TCategorizedData;
   class TEventCollection;
}


class art::TRawDataMappingProcessor : public TProcessor {

public:
   TRawDataMappingProcessor();
   ~TRawDataMappingProcessor();
   virtual void Init(TEventCollection *col);
   virtual void Process();
   
protected:
   InputData<TCategorizedData> fInputCatData;
   OutputData<TClonesArray> fOutput;
   Parameter<Int_t> fCatID;
   Parameter<Int_t> fDataType;
   Parameter<TString> fOutputClass;

   ClassDef(TRawDataMappingProcessor,1) // rawdata mapping (just copy)
};

#endif  // TRAWDATAMAPPINGPROCESSOR_H
