/* $Id:$ */
/**
 * @file   TGraphDrawer.h
 * @date   Created : 2015-04-05 17:04:42 JST
 *   Last Modified : 2015-05-19 17:28:23 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
/* Example. In case that use options[runname,runnumber,TF1 graph(same draw),3 texts]
 *  In processor::Init(),
 *      fGraphDrawer = new TGraphDrawer(name,title,wtopx,wtopy,ww,wh);      
 *      // you can also choose positon and size of canvas. If you use some canvases, you need set canvas name.
 *      fGraphDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFilePath); // option
 *      fGraphDrawer->SetViewHit(fSaveHit);                                 // option
 *      fGraphDrawer->SetRunHeader(fRunHeaders);                            // option
 *
 *  If you want to use runname or runnumber, you need to add
 *      RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,
 *                         TString("runheader"),&fRunHeaders,"TList","TList");
 *  in constructor.
 *
 *  In processor::Process(),
 *      fGraphDrawer->SetGraph(*graph);
 *      fGraphDrawer->SetEvent(fEvt,iHit);             // option
 *      fGraphDrawer->SetTF1(0,value);                 // option
 *      fGraphDrawer->SetTextNDC("text1",1);           // option: add text inserted at Top-Left    position in figure
 *      fGraphDrawer->SetTextNDC("text2",2);           // option: add text inserted at Bottom-Left position in figure
 *      fGraphDrawer->SetTextNDC("text3",5,0.10,0.91); // option: add text inserted at any position you want in figure
 *      fGraphDrawer->Draw();
 *
 *  CAUTION: Please set fWaitTime more than 1.
 */

#ifndef TGRAPHDRAWER_H
#define TGRAPHDRAWER_H

#include <TDataObject.h>
#include <TRunInfo.h>
#include <TList.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>
#include <TText.h>
#include <TDatime.h>
#include <TLine.h>

namespace art {
   class TGraphDrawer;
}

class art::TGraphDrawer  : public TDataObject {

public:
   TGraphDrawer();
   TGraphDrawer(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh);

   virtual ~TGraphDrawer();

   virtual void AddGraph(TGraph &obj);

   virtual void Copy(TObject &obj) const;

   virtual void Clear(Option_t *);

   virtual void Draw(Option_t *opt="");

   virtual void SetCanvas(const char* name, const char* title, Int_t wtopx=0, Int_t wtopy=0, Int_t ww=1000, Int_t wh=1050);

   virtual void SetEvent(Int_t evt, Int_t hit=-1){ fEvt = evt; fHit = hit; }; 

   virtual void SetFigureFile(TString filepath){ fFigFilePath = filepath; }; 

   virtual void SetGraph(TGraph &obj);

   virtual void SetGraphRange(Double_t xmin, Double_t xmax, Double_t ymin, Double_t ymax)
      { fGraph_xmin = xmin; fGraph_xmax = xmax; fGraph_ymin = ymin; fGraph_ymax = ymax; fChGraphRange = 1; };

   //virtual void SetLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
   //   { fLineX1 = x1; fLineY1 = y1; fLineX2 = x2; fLineY2 = y2; }; 

   virtual void SetLine(Double_t x1, Double_t y1, Double_t x2, Double_t y2);

   virtual void SetLine2(Double_t x1, Double_t y1, Double_t x2, Double_t y2);

   virtual void SetOptions(Int_t waittime, Int_t viewevt, Bool_t save, TString filepath);

   virtual void SetRunHeader(TList **runheader){ fRunHeaders = runheader; }

   virtual void SetSaveFlag(Bool_t save){ fSaveFig = save; }; 

   virtual void SetTextNDC(const char* text, Int_t pos=1, Double_t ndc_x=0.10, Double_t ndc_y=0.94);
   // pos: 1(default)->Top-Left, 2->Bottom-Left, 3->Top-Right, 4->Bottom-Right, 5->Anywhere. Set x,y (NDC coordinaties)

   //virtual void SetTF1(Int_t param, Double_t value);
   virtual void SetTF1(const char* fomula);

   virtual void SetViewEvent(Int_t viewevt){ fViewEvt = viewevt; }; 

   virtual void SetViewHit(Int_t viewhit){ fViewHit = viewhit; }; 

   //virtual void SetViewID(Int_t id){ fViewID = id; };

   virtual void SetWaitTime(Int_t waittime){ fWaitTime = waittime; }; 

   static const Int_t kMaxNText = 50;

protected:
   TCanvas      *fCanvas;
   const char   *fCanvasName;  //!
   const char   *fCanvasTitle; //!
   Int_t        fCanvasWtopx;
   Int_t        fCanvasWtopy;
   Int_t        fCanvasWw;
   Int_t        fCanvasWh; 
   TGraph       *fhGraph;
   TGraph       *fhGraph2;
   TF1          *fhTF1;
   Int_t        fNText;
   const char   **fss;         //!->
   Double_t     *fTextX;       //!
   Double_t     *fTextY;       //!
   TText        *ft;
   Int_t        fWaitTime;     // Unit: 0.1 sec
   Int_t        fEvt;
   Int_t        fHit;
   Int_t        fViewEvt;      // Event you want to view or save, if -1: all
   Int_t        fViewHit;      // Hit   you want to view or save, if -1: all
   //Int_t        fViewID;       // ID    you want to view or save, if -1: all
   Bool_t       fSaveFig;      // Save Figure Flag 
   TString      fFigFilePath;  // Figure File Path, default: "../figs/"
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;
   TDatime      *fDatime;
   UInt_t       kDiffTime;
   TLine        *fLine;
   Double_t     fLineX1;
   Double_t     fLineX2;
   Double_t     fLineY1;
   Double_t     fLineY2;
   TLine        *fLine2;
   Double_t     fLine2X1;
   Double_t     fLine2X2;
   Double_t     fLine2Y1;
   Double_t     fLine2Y2;
   Double_t     fGraph_xmin;
   Double_t     fGraph_xmax;
   Double_t     fGraph_ymin;
   Double_t     fGraph_ymax;
   Bool_t       fChGraphRange;

   ClassDef(TGraphDrawer,1);
};
#endif // end of #ifdef TGRAPHDRAWER_H
