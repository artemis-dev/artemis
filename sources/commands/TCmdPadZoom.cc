/**
 * @file   TCmdPadZoom.cc
 * @brief  zoom subpad
 *
 * @date   Created       : 2017-06-19 14:52:37 JST
 *         Last Modified : 2017-06-19 14:53:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdPadZoom.h"

#include <TCatPadManager.h>
#include <TCmdPcd.h>
#include <TCanvas.h>
#include <TList.h>
#include <TFrame.h>

using art::TCmdPadZoom;

ClassImp(TCmdPadZoom)

TCmdPadZoom::TCmdPadZoom()
: fCanvas(NULL)
{
   SetName("pzoom");
   SetTitle("zoom subpad");
}

TCmdPadZoom::~TCmdPadZoom()
{
   delete fCanvas;
}

TCmdPadZoom *TCmdPadZoom::Instance()
{
   static TCmdPadZoom instance;
   return &instance;
}

void TCmdPadZoom::Closed()
{
   fCanvas = NULL;
   TCatPadManager::Instance()->Current()->cd();
}

void TCmdPadZoom::CreateCanvas()
{
   if(!fCanvas) {
      fCanvas = new TCanvas("zoom","zoom",800,800);
      fCanvas->Connect("Closed()","art::TCmdPadZoom",this,"Closed()");
   }
   fCanvas->Clear();
   fCanvas->cd();
}

void TCmdPadZoom::Help()
{
   printf("usage: %s pad_id\n",GetName());
}

Long_t TCmdPadZoom::Cmd(vector<TString> tokens)
{
   if(tokens.size() == 1) {
      return Run(0);
   }

   if(!tokens[1].CompareTo("off")) {
      if (fCanvas) fCanvas->Close();
      return 1;
   }
   
   if (tokens.size() != 2 || !tokens[1].IsDec()) {
      Help();
      return 1;
   }

   return Run(tokens[1].Atoi());
}

Long_t TCmdPadZoom::Run(Int_t id)
{
   if (!gPad || !TCatPadManager::Instance()->HasChild()) return 1;
   if (id) {
      if (id > TCatPadManager::Instance()->GetNumChild()) {
         return 1;
      }
      art::TCmdPcd::Instance()->Run(id);
   }

   TList *const primitives = gPad->GetListOfPrimitives();
   const Int_t n = primitives->GetEntries();
   vector<TString> opts;
   for(Int_t i = 0; i != n; ++i) {
      // DrawOption seems to be reset when new TCanvas is created ...
      // So DrawOptions are saved in opts vector.
      Option_t *opt = primitives->At(i)->GetDrawOption();
      opts.push_back(opt);
   }      
   CreateCanvas();

   for(Int_t i = 0; i != n; ++i) {
      TObject *const obj = primitives->At(i);
      if(obj->InheritsFrom(TFrame::Class())) {
         continue;
      }
      const TString &drawopt =
         TString::Format("%s%s",opts[i].Data(),i?"same":"");
      obj->Draw(drawopt.Data());
   }

   return 1;
}
