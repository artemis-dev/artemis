/**
 * @file   TGainShiftCorrectionProcessor.h
 * @brief  generate gain shift correction table and correction by table 
 *
 * @date   Created       : 2018-07-25 18:16:01 JST
 *         Last Modified : 2018-07-26 22:01:20 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_669B8583_C9B1_4927_B373_73412259EEE0
#define INCLUDE_GUARD_UUID_669B8583_C9B1_4927_B373_73412259EEE0

#include "TProcessor.h"

namespace art {
   class TGainShiftCorrectionProcessor;
   class TEventHeader;
}

class TH2;
class TList;
class TFile;

class art::TGainShiftCorrectionProcessor : public TProcessor {
public:
   typedef enum { ECharge, ETiming } DataType_t;
   
   TGainShiftCorrectionProcessor();
   virtual ~TGainShiftCorrectionProcessor();

   TGainShiftCorrectionProcessor(const TGainShiftCorrectionProcessor& rhs);
   TGainShiftCorrectionProcessor& operator=(const TGainShiftCorrectionProcessor& rhs);
   

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void PostLoop();
   virtual void EndOfRun();
   
   
protected:

   Int_t fDoCreate; // create table without correction
   TString fInputName; // name of input collection
   TClonesArray **fInput; //! input array
   Int_t fDataType; // type of data 0: charge, 1: timing
   Int_t fNumDet; // number of detectors

   TString fDirName; // output directory without run number
   TString fFileNamePrefix; // filename prefix
   TString fTableFileName; // output correction table name
   TString fHistFileName;

   TString fRunHeaderName; // name of runheader
   TList **fRunHeader; //! list of run header


   TString fEventHeaderName; // name of event header
   TEventHeader **fEventHeader; //!

   TFile *fFile; //! histogram file pointer
   
   std::vector<TH2*> fHists; //! histogram for gainshift
   DoubleVec_t fAxisY; // range of Y axis of histograms
   Int_t fEventNumberModulus; // modules for calculate the number of bins in X-axis

private:

   ClassDef(TGainShiftCorrectionProcessor,1) //
};

#endif // INCLUDE_GUARD_UUID_669B8583_C9B1_4927_B373_73412259EEE0
