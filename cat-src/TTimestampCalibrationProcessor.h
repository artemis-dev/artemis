/* @file TTimestampCalibrationProcessor.h
 * @brief timestamp calibration using TAffineConverter
 *
 * @date Create        : 2019-09-09 18:16:19 JST
 *       Last Modified : 2019-09-09 18:30:07 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_E93CA473_624F_4073_931D_FB7A4D8829F7
#define INCLUDE_GUARD_UUID_E93CA473_624F_4073_931D_FB7A4D8829F7

#include "TProcessor.h"

namespace art {
   class TTimestampCalibrationProcessor;
   class TConverterBase;
   class TDataObject;
}


class art::TTimestampCalibrationProcessor : public TProcessor {
public:
   TTimestampCalibrationProcessor();
   virtual ~TTimestampCalibrationProcessor();


   void Process();

protected:
   InputInfo<TClonesArray,TConverterBase> fConverter;
   InputData<TClonesArray,TDataObject> fInput;


   ClassDef(TTimestampCalibrationProcessor,1)
}; // end of TTimestampCalibrationProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_E93CA473_624F_4073_931D_FB7A4D8829F7}
