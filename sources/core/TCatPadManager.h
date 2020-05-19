/* $Id:$ */
/**
 * @file   TCatPadManager.h
 * @date   Created : Feb 06, 2012 18:06:59 JST
 *   Last Modified : 2019-07-27 14:38:12 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPADMANAGER_H
#define TCATPADMANAGER_H

#include <TCanvas.h>

class TPaveLabel;

class TCatPadManager  {
protected:
   TCatPadManager();
   TPaveLabel* GetTitleLabel()  { return fTitleLabel; }
   TPaveLabel* GetCommentLabel()  { return fCommentLabel; }
   
public:
   ~TCatPadManager();
   static TCatPadManager *Instance();
   TVirtualPad *Next();
   TVirtualPad *Previous();
   TVirtualPad *Current();
   TVirtualPad *Get(Int_t idx);
   TVirtualPad *GetCanvas();
   void Closed();
   void MainPadClosed();
   Bool_t HasChild();
   Int_t GetNumChild();
   void CreateCanvas();

   static void SetTitle(const char* title);
   static void SetComment(const char* comment);

   void SetCurrentPadId(Int_t id);
   
   void Divide(Int_t nx, Int_t ny, 
                 Float_t xmargin = 0.01, Float_t ymargin = 0.01);

   TCanvas *fCanvas; //!
   TPad    *fMainPad; //!
   Int_t    fCurrentPadId;
   Int_t    fNumSubPads;

   TPaveLabel *fTitleLabel; //!
   TPaveLabel *fDateLabel; //!
   TPaveLabel *fCommentLabel; //!


   ClassDef(TCatPadManager,1); // pad manager
};
#endif // end of #ifdef TCATPADMANAGER_H
