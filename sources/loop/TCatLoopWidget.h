/* $Id:$ */
/**
 * @file   TCatLoopWidget.h
 * @date   Created : May 03, 2012 22:03:11 JST
 *   Last Modified : May 11, 2012 09:32:33 JST
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATLOOPWIDGET_H
#define TCATLOOPWIDGET_H

#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TGTextEdit.h>
#include <RQ_OBJECT.h>

class TCatLoop;


class TCatLoopWidget  {
   RQ_OBJECT("TCatLoopWidget");
public:
   TCatLoopWidget(const TGWindow *p, Int_t iLoop);
   virtual ~TCatLoopWidget();

   virtual TGCompositeFrame* CreateFrame(TGCompositeFrame *tabFrame);
   virtual void Info(TString line);
   virtual void Resume();
   virtual void Suspend();
private:
   TGMainFrame *fMain;
   TGTextEdit  *fText;
   Int_t        fLoopID;
   TRootEmbeddedCanvas *fEcanvas;
};
#endif // end of #ifdef TCATLOOPWIDGET_H
