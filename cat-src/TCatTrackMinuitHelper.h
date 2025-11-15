/**
 * @file   TCatTrackMinuitHelper.h
 * @brief  helper for track minuit
 *
 * @date   Created       : 2016-07-22 08:30:52 JST
 *         Last Modified : 2016-11-08 11:38:28 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_916A018A_0E92_4346_82B8_7550559DB0CA
#define INCLUDE_GUARD_UUID_916A018A_0E92_4346_82B8_7550559DB0CA

#include "TMinuit.h"
#include "IProcessorHelper.h"
#include <vector>

namespace art {
   class TCatTrackMinuitHelper;
   class TEventCollection;
   class TProcessor;
   class TRangeTableHelper;
   class TCatReadoutPadArray;
   class TCatPulseShape;
}

class TGraph;
class TObjArray;

class art::TCatTrackMinuitHelper : public TMinuit, public IProcessorHelper {
public:
   TCatTrackMinuitHelper();
   virtual ~TCatTrackMinuitHelper();

   TCatTrackMinuitHelper(const TCatTrackMinuitHelper& rhs);
   TCatTrackMinuitHelper& operator=(const TCatTrackMinuitHelper& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Register(TProcessor *proc);

   virtual void SetData(TObjArray *input);


   // parameters are the offset, range and the slope of line
   virtual void SetParameters(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz, Double_t range);
   virtual void GetParameters(Double_t &x0, Double_t &y0, Double_t &z0, Double_t &vx, Double_t &vy, Double_t &vz, Double_t &range);

   // overload of TMinuit::Eval
   virtual Int_t Eval(Int_t npar, Double_t *grad, Double_t &fval, Double_t *par, Int_t flag);
   
   // calculate using start and direction vector with correct magnitude
   virtual Double_t CalcResidual(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz);

   virtual void SetWorkFunction(Double_t wf) { fWorkFunction = wf; }
   virtual void SetRangeTableHelper(TRangeTableHelper *helper) { fRangeTableHelper = helper; }
   virtual void SetReadoutPadArray(TCatReadoutPadArray **arr) { fReadoutPadArray = arr; }

   virtual TCatReadoutPadArray** GetReadoutPadArray() { return fReadoutPadArray; }

protected:
   TRangeTableHelper *fRangeTableHelper; //!

   Double_t fWorkFunction;

   TString fNameReadoutPadArray;
   TCatReadoutPadArray **fReadoutPadArray; //!

   std::vector<TCatPulseShape*> fHits;  //!
   TGraph *fRange2EnergyLoss; //!
   

private:

   ClassDef(TCatTrackMinuitHelper,1) // helper for track minuit
};

#endif // INCLUDE_GUARD_UUID_916A018A_0E92_4346_82B8_7550559DB0CA
