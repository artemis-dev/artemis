/* $Id:$ */
/**
 * @file   DrawEvent.C
 * @date   Created : Feb 09, 2013 16:09:46 JST
 *   Last Modified : Feb 11, 2013 19:16:28 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */

#ifndef __CINT__
#include <TClonesArray.h>
#include <TH2Poly.h>
#include <TTree.h>
#include "TCatFadcRawData.h"
#include "PadDraw.C"
#include <iostream>
using namespace std;
#endif


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
   void DrawEvent(const *char filename, const char *treename = "test", const char *brrname = "atraw") {
      TFile *file = TFile::Open(filename);
      fTree = file->Get(treename);
      if !(fTree) return;
      fTree->SetBranchAddress(brname,fHits);
      fHPad = DrawPad(fNPad,fX,fY);
      fCurrent = 0;
      fCanvas = TCanvas("cDrawEvent","Event display",800,800);
      fCanvas->Draw();
   }

   void Draw(Int_t evtnum = 0) {
      fCurrent = evtnum;
      fTree->GetEntry(fCurrent);
      Int_t nHit = fHit->GetEntries();
      fHPad->ClearBinContents();
      for (Int_t iHit = 0; iHit != nHit; iHit) {
         TCatFadcRawData *d = (TCatFadcRawData*)(*fHit)[iHit];
         fHPad->Fill(x[d->GetID()],y[d->GetID()]);
      }
      fCanvas->Modified();
   }
   ClassDef(DrawEvent,1);
};

