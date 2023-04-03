/* @file TDetectorGeometryUpdateProcessor.cc
 * @brief description
 *
 * @date Create        : 2019-06-13 18:29:09 JST
 *       Last Modified : 2019-06-13 19:10:24 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TDetectorGeometryUpdateProcessor.h"
#include "TDataObject.h"
#include "IPosition.h"
#include "TDetectorGeometryInfo.h"

ClassImp(art::TDetectorGeometryUpdateProcessor)

using art::TDetectorGeometryUpdateProcessor;

TDetectorGeometryUpdateProcessor::TDetectorGeometryUpdateProcessor()
{
   Register(fInputData("Input","name of input array data inherits from IPosition","inputData"));
   Register(fInputInfo("Geometry","name of detector geometry info array","geometry"));
}

TDetectorGeometryUpdateProcessor::~TDetectorGeometryUpdateProcessor()
{
}

void TDetectorGeometryUpdateProcessor::Process()
{
   for (int i = 0, n = fInputData->GetEntriesFast(); i < n; ++i) {
      TDataObject* obj = static_cast<TDataObject*>(fInputData->UncheckedAt(i));
      IPosition* pos = dynamic_cast<IPosition*>(obj);
      Int_t id = obj->GetID();
      TDetectorGeometryInfo *info = static_cast<TDetectorGeometryInfo*>(fInputInfo->UncheckedAt(id));
      pos->SetXYZ(info->X(),info->Y(),info->Z());
   }
}
