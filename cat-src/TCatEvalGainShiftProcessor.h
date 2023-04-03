/* @file TCatEvalGainShiftProcessor.h
 * @brief evaluation of gain shift and obtain gain shift correction
 *
 * @date Create        : 2020-07-27 14:07:46 JST
 *       Last Modified : 2020-08-20 16:18:23 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_6F67085D_726F_4730_9389_D8BFC88A9F0B
#define INCLUDE_GUARD_UUID_6F67085D_726F_4730_9389_D8BFC88A9F0B

#include "TProcessor.h"

namespace art {
   class TCatEvalGainShiftProcessor;
   class TCatReadoutPadArray;
}


class art::TCatEvalGainShiftProcessor : public TProcessor {
public:
   TCatEvalGainShiftProcessor();
   virtual ~TCatEvalGainShiftProcessor();
   virtual void EndOfRun();
protected:
   Parameter<TString> fInputFilename;
   Parameter<TString> fOutputFilename;
   Parameter<TString> fCorrectionFilename;
   Parameter<Double_t> fEventsModulus;
   Parameter<TString> fEventHeaderName;
   Parameter<DoubleVec_t> fChargeRange;
   Parameter<Double_t> fChargeThreshold;
   Parameter<TString> fReadoutPadName;
   Parameter<TString> fInputTrackName;
   Parameter<Double_t> fCountRatioThreshold;
   Parameter<Double_t> fCountThreshold;
   Parameter<Double_t> fDeviationThreshold;
   
   
   
   
   ClassDef(TCatEvalGainShiftProcessor,1)
}; // end of TCatEvalGainShiftProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_6F67085D_726F_4730_9389_D8BFC88A9F0B}
