/* $Id:$ */
/**
 * @file   DrawEvent.C
 * @date   Created : Feb 09, 2013 16:09:46 JST
 *   Last Modified : Feb 11, 2013 19:29:26 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */


#include <TClonesArray.h>
#include <TH2Poly.h>
#include <TTree.h>
#include <TFile.h>
#include <TCanvas.h>
#include "TCatFadcRawData.h"
#include "PadDraw.C"
#include <iostream>
using namespace std;

class DrawEvent {
private:
   TTree* fTree;
   TClonesArray *fHits;
   TH2Poly *fHPad;
   Int_t fNPad;
   Double_t *fX;
   Double_t *fY;
   Int_t fCurrent;
   TCanvas *fCanvas;

public:
   DrawEvent(){;}

   DrawEvent(const char *filename, const char *treename = "test", const char *brname = "atraw") {
      TFile *file = TFile::Open(filename);
      fTree = (TTree*)file->Get(treename);
      if (!fTree) return;
      fHits = NULL;
      fTree->SetBranchAddress(brname,&fHits);
      fHPad = PadDraw(fNPad,fX,fY);
      fCurrent = 0;
      fCanvas = new TCanvas("cDrawEvent","Event display",800,800);
      fCanvas->Draw();
      fHPad->Draw();
   }

   void Draw(Int_t evtnum = 0) {
      fCurrent = evtnum;
      fTree->GetEntry(fCurrent);
      Int_t nHit = fHits->GetEntries();
      fHPad->ClearBinContents();
      printf("evtnum = %d, nHit = %d\n",fCurrent,nHit);
      for (Int_t iHit = 0; iHit != nHit; iHit++) {
         TCatFadcRawData *d = (TCatFadcRawData*)(*fHits)[iHit];
         fHPad->Fill(fX[d->GetID()],fY[d->GetID()]);
      }
      fHPad->Draw();
      fCanvas->Modified();
   }
   ClassDef(DrawEvent,1);
};

