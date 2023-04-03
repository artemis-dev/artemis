/* $Id:$ */
/**
 * @file   TCatAtPedestalCalculator.h
 * @date   Created : Sep 19, 2013 09:19:43 JST
 *   Last Modified : Sep 19, 2013 11:37:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATPEDESTALCALCULATOR_H
#define TCATATPEDESTALCALCULATOR_H

#include <TProcessor.h>

class TH2F;

namespace art {
   class TCatAtPedestalCalculator;
   class TSegmentedData;
}

class art::TCatAtPedestalCalculator  : public art::TProcessor {

public:
   TCatAtPedestalCalculator();
   virtual ~TCatAtPedestalCalculator();

protected:
   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void BeginOfRun();
   virtual void EndOfRun();

   // variables for the steering parameter
   StringVec_t      fInputColName;
   IntVec_t         fSegID;
   Int_t            fMaxGeo;
   TString          fOutputRootFile;
   TString          fOutputTextFile;
   
   TSegmentedData **fSegments; //!

   
   TH2F            *fHistPedestal;
   
   ClassDef(TCatAtPedestalCalculator,1)
};
#endif // end of #ifdef TCATATPEDESTALCALCULATOR_H
