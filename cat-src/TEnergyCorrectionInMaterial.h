/* @file TEnergyCorrectionInMaterial.h
 * @brief energy correction in material with range table
 *
 * @date Create        : 2019-06-08 22:45:46 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_1ED4A925_8BAF_42EC_9842_F70E503AAE3A
#define INCLUDE_GUARD_UUID_1ED4A925_8BAF_42EC_9842_F70E503AAE3A

#include "TProcessor.h"

namespace art {
   class TEnergyCorrectionInMaterial;
   class TRangeTableHelper;
   class ICharge;
   class ITrack;
   class TDetectorGeometryInfo;
}


class art::TEnergyCorrectionInMaterial : public TProcessor {
public:
   TEnergyCorrectionInMaterial();
   virtual ~TEnergyCorrectionInMaterial();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   InputInfo<TRangeTableHelper> fRangeTable;
   InputData<TClonesArray,ICharge> fInputCharge;
   InputData<TClonesArray,ITrack> fInputTrack;
   OutputData<TClonesArray,ICharge> fOutputCharge;
   InputInfo<TClonesArray,TDetectorGeometryInfo> fGeometry;
   Parameter<Double_t> fOverrideDeadLayer;
   
   
   ClassDef(TEnergyCorrectionInMaterial,1)
}; // end of TEnergyCorrectionInMaterial

#endif // #ifndef INCLUDE_GUARD_UUID_1ED4A925_8BAF_42EC_9842_F70E503AAE3A}
