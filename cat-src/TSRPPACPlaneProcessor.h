/**
 * @file   TSRPPACPlaneProcessor.h
 * @brief  Processor for SR-PPAC
 *
 * @date   Created       : 2019-02-07 03:40:33 DST
 *         Last Modified : 2019-07-22 13:57:45 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_9132A2D4_F2C7_4CB0_A736_1F3C20C2793B
#define INCLUDE_GUARD_UUID_9132A2D4_F2C7_4CB0_A736_1F3C20C2793B

#include "TProcessor.h"
#include "TEventCollection.h"
#include "TTimingChargeData.h"
#include "TSRPPACPlaneData.h"


namespace art {
   class TSRPPACPlaneProcessor;
   class TSRPPACParameter;
}

class TClonesArray;

class art::TSRPPACPlaneProcessor : public TProcessor {
public:
   TSRPPACPlaneProcessor();
   virtual ~TSRPPACPlaneProcessor();
   
   virtual void Init(TEventCollection *col);
   virtual void Process();
   Int_t GetNStrip() const { return fNStrip; }
   Double_t GetStripWidth() const { return fStripWidth; }
   Double_t GetCenter() const { return fCenter; }
   Double_t GetDetOffset() const { return fDetOffset; }
   Double_t GetZ() const { return fZ; }
   Bool_t IsTurned() const { return fTurned; }
   
protected:
   TString fNameInput;
   TString fNameOutput;
   TString fParameterName;
   TClonesArray **fInput;//!
   TClonesArray *fOutput;//!
   TSRPPACParameter *fParameter;
   Int_t fVerbose;
   Int_t fNStrip;
   Double_t fStripWidth;
   Double_t fCenter;
   Double_t fDetOffset;
   Double_t fZ;
   Bool_t fTurned;
   
private:

   ClassDef(TSRPPACPlaneProcessor,1) // Processor for SR-PPAC
};

#endif // INCLUDE_GUARD_UUID_9132A2D4_F2C7_4CB0_A736_1F3C20C2793B
