/**
 * @file   TArtCanvas.cc
 * @brief  artemis canvas inheriting from TCanvas
 *
 * @date   Created       : 2017-06-26 01:51:22 JST
 *         Last Modified : 2017-06-28 17:18:53 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TArtCanvas.h"
#include <TPaveLabel.h>
#include <TDatime.h>
#include <TClass.h>

using art::TArtCanvas;

ClassImp(TArtCanvas)

TArtCanvas::TArtCanvas(Bool_t build)
: TCanvas(build), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fCommentLabel(NULL)
{
   // guard for ls only or just instantiation from Key (without drawing) 
   if (!build || TClass::IsCallingNew() != TClass::kRealNew) {
      return;
   }
   ConstructMainPad();
}

TArtCanvas::TArtCanvas(const char* name, const char *title, Int_t form)
   : TCanvas(name,title,form), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fCommentLabel(NULL)
{
   ConstructMainPad();
}
TArtCanvas::TArtCanvas(const char* name, const char *title, Int_t ww, Int_t wh)
   : TCanvas(name,title,ww,wh), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fCommentLabel(NULL)
{
   ConstructMainPad();
}
TArtCanvas::TArtCanvas(const char *name, const char *title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
   : TCanvas(name,title,wtopx,wtopy,ww,wh), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fCommentLabel(NULL)
{
   ConstructMainPad();
}
TArtCanvas::TArtCanvas(const char *name, Int_t ww, Int_t wh, Int_t winid)
   : TCanvas(name,ww,wh,winid), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fCommentLabel(NULL)
{
   ConstructMainPad();
}

TArtCanvas::~TArtCanvas()
{
}

TArtCanvas::TArtCanvas(const TArtCanvas& rhs)
{
}

TArtCanvas& TArtCanvas::operator=(const TArtCanvas& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TArtCanvas::Divide(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin, Int_t color)
{
   fMainPad->Divide(nx,ny,xmargin,ymargin,color);
}



void TArtCanvas::Clear(Option_t *option)
{
   if (!fMainPad) TCanvas::Clear(option);
   fMainPad->Clear(option);
}

void TArtCanvas::ConstructMainPad()
{
   if (!fMainPad) {
      cd(0);
      fTitleLabel = new TPaveLabel(0.1,0.96,0.9,0.99,"");      
      fDateLabel  = new TPaveLabel(0.5,0.91,0.9,0.95,"");
      fDateLabel->SetBorderSize(0);
      fDateLabel->SetFillStyle(0);
      fCommentLabel  = new TPaveLabel(0.1,0.91,0.5,0.95,"");
      fCommentLabel->SetBorderSize(0);
      fCommentLabel->SetFillStyle(0);
      fTitleLabel->Draw();
      fDateLabel->Draw();
      fCommentLabel->Draw();
      TCanvas::Divide(1,1);
      fMainPad = cd(1);
      if (fMainPad) {
         fMainPad->SetPad(0.05,0.01,0.95,0.91);
      }
   }
   if (fMainPad) UpdateDateLabel();
}

#if 0
void TArtCanvas::Update()
{
   if (!fMainPad)   {
      TCanvas::Update();
      return;
   }
   printf("%s : update\n",GetName());
   TDatime now;
   fDateLabel->SetLabel(now.AsString());

   TCanvas::Modified();
   TCanvas::Update();
}
#endif

void TArtCanvas::SetTitleLabel(const char* title)
{
   fTitleLabel->SetLabel(title);
}

void TArtCanvas::SetCommentLabel(const char* comment)
{
   fCommentLabel->SetLabel(comment);
}

void TArtCanvas::UpdateDateLabel()
{
   TDatime now;
   fDateLabel->SetLabel(now.AsString());
   Modified();
   Update();
}
