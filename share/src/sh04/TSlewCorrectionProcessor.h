/**
 * @file   TSlewCorrectionProcessor.h
 * @brief  slew correction for tof
 *
 * @date   Created       : 2014-11-07 13:33:26 JST
 *         Last Modified : 2018-02-04 12:04:47 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_88CED724_EE2B_4306_BDDA_EFD097041876
#define INCLUDE_GUARD_UUID_88CED724_EE2B_4306_BDDA_EFD097041876

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TSlewCorrectionProcessor;
   }
}

class TClonesArray;

class art::sh04::TSlewCorrectionProcessor : public TProcessor {
public:
   TSlewCorrectionProcessor();
   virtual ~TSlewCorrectionProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString        fInputColName;
   TClonesArray **fInputCol; //!
   TString        fChargeColName;
   TClonesArray **fChargeCol; //!
   Int_t          fChargeDetID;
   Float_t        fParameter1f;
   Float_t        fParameter1b;
   Float_t        fParameter2f;
   Float_t        fParameter2b;

   /* output */
   TString        fOutputColName;
   TClonesArray  *fOutputCol; //!

private:
   TSlewCorrectionProcessor(const TSlewCorrectionProcessor&);
   TSlewCorrectionProcessor& operator=(const TSlewCorrectionProcessor&);

   ClassDef(TSlewCorrectionProcessor,1) // slew correction for tof
};

#endif // INCLUDE_GUARD_UUID_88CED724_EE2B_4306_BDDA_EFD097041876
