/**
 * @file   TWalkCorrectionProcessor.h
 * @brief  walk correction for TTimingChargeData
 *
 * @date   Created       : 2014-06-17 02:56:10 JST
 *         Last Modified : 2018-02-04 11:46:44 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_FCC0553D_B2B4_4EA9_A012_EE31A914499D
#define INCLUDE_GUARD_UUID_FCC0553D_B2B4_4EA9_A012_EE31A914499D

#include "TProcessor.h"

namespace art {
   class TWalkCorrectionProcessor;
}

class TClonesArray;

class art::TWalkCorrectionProcessor : public TProcessor {
public:
   TWalkCorrectionProcessor();
   virtual ~TWalkCorrectionProcessor();

   TWalkCorrectionProcessor(const TWalkCorrectionProcessor& rhs);
   TWalkCorrectionProcessor& operator=(const TWalkCorrectionProcessor& rhs);

   virtual void Process();

protected:
   TString fInputColName;
   TString fOutputColName;
   TString fConverterName;

   TClonesArray **fInput; //! input collection
   TClonesArray  *fOutput; //! output collection
   TClonesArray **fConverter; //! converter
private:

   ClassDef(TWalkCorrectionProcessor,1) // walk correction for TTimingChargeData
};

#endif // INCLUDE_GUARD_UUID_FCC0553D_B2B4_4EA9_A012_EE31A914499D
