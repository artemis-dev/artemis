/**
 * @file   TTimingChargeMappingProcessor.h
 * @brief
 *
 * @date   Created:       2013-08-12 12:36:40
 *         Last Modified: 2023-03-14 12:32:10 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMINGCHARGEMAPPINGPROCESSOR_H
#define TTIMINGCHARGEMAPPINGPROCESSOR_H

#include <TProcessor.h>

namespace art{
   class TTimingChargeMappingProcessor;
   class TCategorizedData;
}

class TClonesArray;

class art::TTimingChargeMappingProcessor : public TProcessor {

public:
   // Default constructor
   TTimingChargeMappingProcessor();
   virtual ~TTimingChargeMappingProcessor();

   virtual void Init(TEventCollection*);
   virtual void Process();

protected:

   Parameter<Int_t> fDoRequireBoth; // flag if requiring both of charge and timing 
   TString            fInputColName;
   TString            fOutputColName;
   TCategorizedData **fCategorizedData; //!
   TClonesArray      *fPlastic; //!

   Int_t  fCatID;        // default: 21 (spla in anapaw)
   Int_t  fChargeType;   // 0: V1190 width (default), 1: QDC (V792 etc)
   Int_t  fChargeTypeID; // used only when fChargeType is QDC
   Int_t  fTimingTypeID; // Typeid for timing (& width when fChargeType is V1190width)
   Bool_t fTrailingComesFirst; // F: TL->TT (default), T: TT->TL (for QTC etc.)

   Bool_t fIsSparse;

private:
   // Copy constructor (prohibited)
   TTimingChargeMappingProcessor(const TTimingChargeMappingProcessor&);
   // Assignment operator (prohibited)
   TTimingChargeMappingProcessor& operator=(const TTimingChargeMappingProcessor&);

   void MapEdgedTime(const TObjArray*);
   void MapPairedTimeCharge(const TObjArray*);
   void MapTimeCharge(const TObjArray*, const TObjArray*);

   enum EChargeTypeID { kWIDTH, kQDC, kHINP,};

   ClassDef(TTimingChargeMappingProcessor,0) // processor for mapping timine and charge data
};

#endif // TTIMINGCHARGEMAPPINGPROCESSOR_H
