/**
 * @file   TMWDCPlaneCalibrationTask.h
 * @brief  task object for MWDC Plane calibration
 *
 * @date   Created       : 2014-04-02 10:20:25 JST
 *         Last Modified : 2014-04-04 12:35:27 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90
#define INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90

#include <TTask.h>

namespace art {
   class TMWDCPlaneCalibrationTask;
   class TGMWDCCalibration;
}

class TH1D;

class art::TMWDCPlaneCalibrationTask : public TTask {
public:
   TMWDCPlaneCalibrationTask();
   TMWDCPlaneCalibrationTask(const char* name, const char* title);
   virtual ~TMWDCPlaneCalibrationTask();

   TMWDCPlaneCalibrationTask(const TMWDCPlaneCalibrationTask& rhs);
   TMWDCPlaneCalibrationTask& operator=(const TMWDCPlaneCalibrationTask& rhs);

   void AddHist(TH1D *h);

   void Exec(Option_t *opt = "");
   void Browse(TBrowser *b);

protected:
   art::TGMWDCCalibration *fMC; //!
   TObjArray *fHists; //->

   ClassDef(TMWDCPlaneCalibrationTask,1) // task object for MWDC Plane calibration
};

#endif // INCLUDE_GUARD_UUID_04D9C824_15F0_4302_802C_ACB8D5314E90
