/**
 * @file   TSRPPACPlaneCalibrationTask.h
 * @brief  task object for SRPPAC Plane calibration
 * Based on TMWDCPlaneCalibration.h
 *
 * @date   Created      : 2019-08-30 18:23:41 JST (ota)
 *         Last Modified: 2019-08-30 18:23:54 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 OTA Shinsuke
 */

#ifndef INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90
#define INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90

#include <TTask.h>

namespace art {
   class TSRPPACPlaneCalibrationTask;
   class TGSRPPACCalibration;
}

class TH1D;

class art::TSRPPACPlaneCalibrationTask : public TTask {
public:
   TSRPPACPlaneCalibrationTask();
   TSRPPACPlaneCalibrationTask(const char* name, const char* title);
   virtual ~TSRPPACPlaneCalibrationTask();

   TSRPPACPlaneCalibrationTask(const TSRPPACPlaneCalibrationTask& rhs);
   TSRPPACPlaneCalibrationTask& operator=(const TSRPPACPlaneCalibrationTask& rhs);

   void AddHist(TH1D *h);

   void Exec(Option_t *opt = "");
   void Browse(TBrowser *b);

protected:
   art::TGSRPPACCalibration *fMC; //!
   TObjArray *fHists; //->

   ClassDef(TSRPPACPlaneCalibrationTask,1) // task object for SRPPAC Plane calibration
};

#endif // INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90
