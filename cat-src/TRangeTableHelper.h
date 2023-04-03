/**
 * @file   TRangeTableHelper.h
 * @brief  helper for dealing range table
 *
 * @date   Created       : 2016-07-21 06:34:34 JST
 *         Last Modified : 2020-04-29 17:41:59 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_54EAF4E8_1879_445C_AD19_6E53C4BB6103
#define INCLUDE_GUARD_UUID_54EAF4E8_1879_445C_AD19_6E53C4BB6103

#include "TNamed.h"
#include <IProcessorHelper.h>
#include <TSpline.h>
#include "TMath.h"

namespace art {
   class TRangeTableHelper;
   class TEventCollection;
   class TProcessor;
}

class TGraph;


class art::TRangeTableHelper : public TNamed, public IProcessorHelper {
public:
   TRangeTableHelper();
   TRangeTableHelper(const char *name, const char *title);
   virtual ~TRangeTableHelper();

   TRangeTableHelper(const TRangeTableHelper& rhs);
   TRangeTableHelper& operator=(const TRangeTableHelper& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Register(TProcessor *proc);

   virtual TGraph *GetR2DE() const { return fR2DE; }
   virtual TGraph *GetDE2R() const { return fDE2R; }
   virtual TGraph *GetR2E() const { return fR2E; }
   virtual TGraph *GetE2R() const { return fE2R; }

   virtual void SetFileName(const char* filename) { fFileName = filename; }
   virtual const char* GetFileName() { return fFileName.Data(); }

   virtual void SetTKECutoff(Double_t cutoff) { MayNotUse("SetTKECutoff");  }

   virtual Double_t R2E(Double_t range) const { return range > 0 ? fSpR2E->Eval(range) : 0. ; }
   virtual Double_t R2DE(Double_t range) const { return range > 0 ? fSpR2DE->Eval(range) : 0. ; }
   virtual Double_t E2R(Double_t tke) const { return tke > 0 ? fSpE2R->Eval(tke) : 0.; }


   virtual Double_t GetMaxRange() const { MayNotUse("GetMaxRange"); return TMath::QuietNaN(); }

   virtual void SetRangeStep(Double_t rangeStep) { MayNotUse("SetRangeStep"); }
   virtual Double_t GetRangeSte() const { MayNotUse("GetRangeSte"); return TMath::QuietNaN(); }

protected:

   TString fFileName;

   TGraph  *fDE2R; //!
   TGraph  *fR2DE; //!
   TGraph  *fR2E; //!
   TGraph  *fE2R; //!

   TSpline3 *fSpDE2R; //!
   TSpline3 *fSpR2DE; //!
   TSpline3 *fSpR2E;  //!
   TSpline3 *fSpE2R;  //!


   TString fRangeTableName;

private:

   ClassDef(TRangeTableHelper,1) // helper for dealing range table
};

#endif // INCLUDE_GUARD_UUID_54EAF4E8_1879_445C_AD19_6E53C4BB6103
