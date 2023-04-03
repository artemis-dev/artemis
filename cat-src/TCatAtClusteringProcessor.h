/**
 * @file   TCatAtClusteringProcessor.h
 * @brief  clustering for tracking detector
 *
 * @date   Created       : 2015-12-18 11:38:20 JST
 *         Last Modified : 2016-08-31 20:48:31 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 *    
 */

#ifndef TCATATCLUSTERINGPROCESSOR_H
#define TCATATCLUSTERINGPROCESSOR_H

#include <TProcessor.h>
#include "TPulseDrawer.h"
#include <TGraph.h>
#include <TF1.h>
#include <TH2F.h>
#include <stdio.h>
#include <TCanvas.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TCatAtClusteringProcessor;
   class TEventHeader;
}

class art::TCatAtClusteringProcessor : public TProcessor {
public:
   TCatAtClusteringProcessor();
   virtual ~TCatAtClusteringProcessor();

   TCatAtClusteringProcessor(const TCatAtClusteringProcessor& rhs);
   TCatAtClusteringProcessor& operator=(const TCatAtClusteringProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray *fOutput;
   TClonesArray *fOutputBeam;
   TClonesArray *fExcludeEvtOutput;
   StringVec_t  fInputColName;
   TString      fOutputColName;
   TString      fOutputBeamColName;
   TString      fOutputExcludeEvtColName;
   Int_t        fEvt;
   Int_t        fWaitTime;
   TString      fLogFilePath;
   TString      fLogFileName;
   Bool_t       fSavePulse;
   Bool_t       fDrawPulse;
   Bool_t       fSaveFig;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   Bool_t       fSaveHist;
   Bool_t       fSaveExcludeEvt;
   TString      fFigFile;
   TString      fFigFilePath;
   TString      fAddFigFilePath;
   Int_t        fSaveEvt;

   TCanvas      *fCanvas;

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TString      fNameEventHeader;
   TEventHeader **fEventHeader; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;

   TPulseDrawer *fPulseDrawer;
   Int_t        fCanvasX;
   Int_t        fCanvasY;
   Int_t        fCanvasH;
   Int_t        fCanvasW;

   Int_t        fMinNHits;

   TH1F         *fhNumOfCluster;
   TH1F         *fhClusterSize;
   TH1F         *fhClusterQ;

   Int_t        fCutHits;
   Bool_t fUseAllCluster; // flag to use all the clusters

   ClassDef(TCatAtClusteringProcessor,1) // clustering for tracking detector
};

#endif // TCATATCLUSTERINGPROCESSOR_H
