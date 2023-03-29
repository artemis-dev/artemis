/**
 * @file   TPIDwSinglePPACPlaneProcessor.h
 * @brief  pid with mwdc single plane
 *
 * @date   Created       : 2018-07-12 17:36:58 JST
 *         Last Modified : 2019-11-20 19:42:49 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_EB5C7708_88E9_4600_BBED_A3AC03559621_SR
#define INCLUDE_GUARD_UUID_EB5C7708_88E9_4600_BBED_A3AC03559621_SR

#include "TProcessor.h"
//#include "TSRPPACParameter.h"
//#include "TSRPPACPlaneData.h"

namespace art {
   class TPIDwSinglePPACPlaneProcessor;
   class TSRPPACParameter;
   class TSRPPACPlaneData;
   
}

class art::TPIDwSinglePPACPlaneProcessor : public TProcessor {
public:
   TPIDwSinglePPACPlaneProcessor();
   virtual ~TPIDwSinglePPACPlaneProcessor();

   TPIDwSinglePPACPlaneProcessor(const TPIDwSinglePPACPlaneProcessor& rhs);
   TPIDwSinglePPACPlaneProcessor& operator=(const TPIDwSinglePPACPlaneProcessor& rhs);

   virtual void Init(TEventCollection *col);

   virtual void Process();

   virtual void CalcZAQ(Double_t tof, Double_t x, Double_t& z, Double_t& aq, Double_t &br35, Double_t &br57);

protected:
   InputInfo<TSRPPACParameter> fPPACParam;
   InputData<TClonesArray,TSRPPACPlaneData> fInput;
   Parameter<Int_t> fDoCheckTimestamp;
   

   TString fTofName; // tof name
   TClonesArray **fTOFs; //!

   TString fOutputName; // output name (aq)
   TClonesArray *fOutput;  //!
   TString fOutputZName; // output name (Z)
   TClonesArray *fOutputZ;  //!

   TString fOutputBrho1Name; // 
   TString fOutputBrho2Name; //

   TClonesArray *fBrho1; //!
   TClonesArray *fBrho2; //!

   DoubleVec_t fDispersion;

   DoubleVec_t fBrho0; // two brhos
   DoubleVec_t fLength; // two brhos
   DoubleVec_t fDegThick; // d = p0 + p1 * x

   

private:

   ClassDef(TPIDwSinglePPACPlaneProcessor,1) // pid with srppac single plane
};

#endif // INCLUDE_GUARD_UUID_EB5C7708_88E9_4600_BBED_A3AC03559621_SR
