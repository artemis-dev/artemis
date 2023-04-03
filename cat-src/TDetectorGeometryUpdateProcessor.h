/* @file TDetectorGeometryUpdateProcessor.h
 * @brief Geometry update processor
 *
 * @date Create        : 2019-06-13 18:25:47 JST
 *       Last Modified : 2019-06-13 18:36:03 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_AA75958D_172D_4CE4_9EBC_3F5E2F954F64
#define INCLUDE_GUARD_UUID_AA75958D_172D_4CE4_9EBC_3F5E2F954F64

#include "TProcessor.h"

namespace art {
   class TDetectorGeometryUpdateProcessor;
   class IPosition;
   class TDetectorGeometryInfo;
}

class TCollection;


class art::TDetectorGeometryUpdateProcessor : public TProcessor {
public:
   TDetectorGeometryUpdateProcessor();
   virtual ~TDetectorGeometryUpdateProcessor();


   virtual void Process();
   
   
protected:
   InputData<TClonesArray,IPosition> fInputData;
   InputInfo<TClonesArray,TDetectorGeometryInfo> fInputInfo;
   

   ClassDef(TDetectorGeometryUpdateProcessor,1)
}; // end of TDetectorGeometryUpdateProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_AA75958D_172D_4CE4_9EBC_3F5E2F954F64}
