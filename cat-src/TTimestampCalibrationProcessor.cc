/* @file TTimestampCalibrationProcessor.cc
 * @brief timestamp calibration with TAffineConverter
 *
 * @date Create        : 2019-09-09 18:18:09 JST
 *       Last Modified : 2019-09-09 19:06:56 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TTimestampCalibrationProcessor.h"
#include "TConverterBase.h"
#include "TDataObject.h"

ClassImp(art::TTimestampCalibrationProcessor)

using art::TTimestampCalibrationProcessor;

TTimestampCalibrationProcessor::TTimestampCalibrationProcessor()
{
   Register(fConverter("Converter","Name of converter","converter"));
   Register(fInput("Input","name of input collection","input"));
   fConverter.SetDoAuto(true);
   fInput.SetDoAuto(true);
}

TTimestampCalibrationProcessor::~TTimestampCalibrationProcessor()
{
}


void TTimestampCalibrationProcessor::Process() {
   const int n = fInput->GetEntriesFast();
   if (n == 0) return;

   for (int i = 0; i < n; ++i) {
      TDataObject *data = static_cast<TDataObject*>(fInput->UncheckedAt(i));
      const int id = data->GetID();
      TConverterBase *converter;
      if (fConverter->GetEntriesFast() < id ||
          (converter = static_cast<TConverterBase*>(fConverter->At(id))) == NULL) {
         if (fVerboseLevel > 0) Warning("Process","Converter is not set to id = %d",id);
         continue;
      }
      data->SetTimestamp(converter->Convert(data->GetTimestamp()));
   }
}

