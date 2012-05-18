/* $Id:$ */
/**
 * @file   TCatLoopWidget.cc
 * @date   Created : May 03, 2012 22:03:31 JST
 *   Last Modified : May 12, 2012 09:33:10 JST
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoopWidget.h"
#include <TCatLoop.h>

#include <TGFrame.h>
#include <TGButton.h>
#include <TGTextEdit.h>
#include <TGTab.h>

#include <TCatLoopManager.h>

TCatLoopWidget::TCatLoopWidget(const TGWindow *p,Int_t iLoop)
{
   if (!TCatLoopManager::Instance()->GetLoop(iLoop)) return;
   fLoopID = iLoop;
   TCatLoopManager::Instance()->GetLoop(iLoop)->AddWidget(this);
}
TCatLoopWidget::~TCatLoopWidget()
{
}


TGCompositeFrame *TCatLoopWidget::CreateFrame(TGCompositeFrame *tabFrame)
{
   TGHorizontalFrame *hframe = new TGHorizontalFrame(tabFrame,300,40);
   TGTextButton *btresume = new TGTextButton(hframe,"&Resume");
   btresume->Connect("Clicked()","TCatLoopWidget",this,"Resume()");
   hframe->AddFrame(btresume,new TGLayoutHints(kLHintsCenterX,2,2,3,4));
   TGTextButton *btsuspend = new TGTextButton(hframe,"&Suspend");
   btsuspend->Connect("Clicked()","TCatLoopWidget",this,"Suspend()");
   hframe->AddFrame(btsuspend,new TGLayoutHints(kLHintsCenterX,2,2,3,4));

   fText = new TGTextEdit(tabFrame,300,250);
   tabFrame->AddFrame(fText,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
   tabFrame->AddFrame(hframe,new TGLayoutHints(kLHintsCenterX,2,2,2,2));
   return hframe;
}

void TCatLoopWidget::Info(TString line)
{
   fText->AddLine(line);
}

void TCatLoopWidget::Resume()
{
   TCatLoopManager::Instance()->Resume(fLoopID);
}

void TCatLoopWidget::Suspend()
{
   TCatLoopManager::Instance()->Suspend(fLoopID);
}
