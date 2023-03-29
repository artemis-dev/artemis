/* $Id:$ */
/**
 * @file   TCatRegionHitsDivider.h
 * @date   Created : Aug 31, 2016 16:31:36 JST
 *   Last Modified : 2018-02-04 13:18:17 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#ifndef TCATREGIONHITSDIVIDER_H
#define TCATREGIONHITSDIVIDER_H

#include <TProcessor.h>
#include <TRunInfo.h>

namespace art {
   class TCatRegionHitsDivider;
   class TEventHeader;
}

class TClonesArray;

class art::TCatRegionHitsDivider  : public TProcessor {

public:
   TCatRegionHitsDivider();
   ~TCatRegionHitsDivider();

   TCatRegionHitsDivider(const TCatRegionHitsDivider& rhs);
   virtual TCatRegionHitsDivider& operator=(const TCatRegionHitsDivider& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray *fOutputRecoil; //!
   TClonesArray *fOutputBeam; //!
   StringVec_t  fInputColName;
   TString      fOutputRecoilColName;
   TString      fOutputBeamColName;
   Int_t        fEvt;
   TString      fLogFilePath;
   TString      fLogFileName;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   Bool_t       fSavePulse;
   TString      fFigFile;
   TString      fFigFilePath;
   TString      fAddFigFilePath;

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TString      fNameEventHeader;
   TEventHeader **fEventHeader; //!
   TRunInfo     *fRunInfo; //!
   TString      fRunName;
   Int_t        fRunNumber;

   ClassDef(TCatRegionHitsDivider,1)
};
#endif // end of #ifdef TCATREGIONHITSDIVIDER_H
