/* $Id:$ */
/**
 * @file   TPulseDrawer.h
 * @date   Created : 2015-03-12 21:08:33 JST
 *   Last Modified : 2015-04-16 11:28:11 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
/* Example. In case that use options[runname,runnumber,TF1 graph(same draw),3 texts]
 *  In processor::Init(),
 *      fPulseDrawer = new TPulseDrawer(name,title,wtopx,wtopy,ww,wh);      
 *      // you can also choose positon and size of canvas. If you use some canvases, you need set canvas name.
 *      fPulseDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFilePath); // option
 *      fPulseDrawer->SetViewHit(fSaveHit);                                 // option
 *      fPulseDrawer->SetRunHeader(fRunHeaders);                            // option
 *
 *  If you want to use runname or runnumber, you need to add
 *      RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,
 *                         TString("runheader"),&fRunHeaders,"TList","TList");
 *  in constructor.
 *
 *  In processor::Process(),
 *      TObject *hitobj = hit;                         // hit: object inherited from art::TCatPulseShape
 *      fPulseDrawer->SetPulse(*hitobj);
 *      fPulseDrawer->SetEvent(fEvt,iHit);             // option
 *      fPulseDrawer->SetTF1(0,value);                 // option
 *      fPulseDrawer->SetTextNDC("text1",1);           // option: add text inserted at Top-Left    position in figure
 *      fPulseDrawer->SetTextNDC("text2",2);           // option: add text inserted at Bottom-Left position in figure
 *      fPulseDrawer->SetTextNDC("text3",5,0.10,0.91); // option: add text inserted at any position you want in figure
 *      fPulseDrawer->Draw();
 */

#ifndef TPULSEDRAWER_H
#define TPULSEDRAWER_H

#include "TGraphDrawer.h"

namespace art {
   class TGraphDrawer;
   class TPulseDrawer;
}

class art::TPulseDrawer  : public TGraphDrawer {

public:
   TPulseDrawer();
   TPulseDrawer(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh);

   virtual ~TPulseDrawer();

   virtual void SetPulse(TObject &obj); 

   virtual void SetViewID(Int_t id){ fViewID = id; };

protected:
   Int_t        fViewID;       // ID    you want to view or save, if -1: all

   ClassDef(TPulseDrawer,1);
};
#endif // end of #ifdef TPULSEDRAWER_H

