/**
 * @file   TCmdUpdateCanvas.cc
 * @brief  update canvas and its child once or automatically
 *
 * @date   Created       : 2014-06-26 13:54:04 JST
 *         Last Modified : 2020-11-27 00:21:23 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TCmdUpdateCanvas.h"
#include <TVirtualPad.h>
#include <TROOT.h>
#include <TPad.h>
#include <TThread.h>
#include "TSystem.h"

using art::TCmdUpdateCanvas;

const Int_t TCmdUpdateCanvas::kDefaultInterval = 2000; // ms

ClassImp(TCmdUpdateCanvas)

TCmdUpdateCanvas::TCmdUpdateCanvas()
{
   SetName("updatecanvas");
   SetTitle("update canvas and its child once or automatically");
}

TCmdUpdateCanvas::~TCmdUpdateCanvas()
{
}

TCmdUpdateCanvas::TCmdUpdateCanvas(const TCmdUpdateCanvas& rhs)
{
}

TCmdUpdateCanvas& TCmdUpdateCanvas::operator=(const TCmdUpdateCanvas& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdUpdateCanvas::Cmd(vector<TString> tokens)
{
   Int_t nTokens = tokens.size();
   TString name;
   Int_t  interval = 0;
   TVirtualPad *pad = NULL;
   if (nTokens > 1) interval = tokens[1].IsNull() ? 0 : TMath::Abs((tokens[1].Atoi()));
   if (nTokens > 2) name = tokens[2];
   if (!name.IsNull() && name != "!") {
      pad = (TVirtualPad*)gROOT->FindObjectAny(name);
   } else {
      pad = gPad;
   }
   if (pad == NULL) {
      printf("no active canvas or pad is found (name = '%s')\n",name.Data());
      Help();
      return 1;
   }
   return Run(pad,interval);
}

Long_t TCmdUpdateCanvas::Run(TVirtualPad *pad, Int_t interval)
{
   for (std::list<UpdateArg_t>::iterator it = fUpdateArgs.begin(); it != fUpdateArgs.end();) {
      UpdateArg_t args = *it;
      if (args.fPad == pad) { 
         if (interval) {
            printf("pad %s is already registered to update\n",pad->GetName());
         } else {
            *args.fUpdate = kFALSE;
            args.fThread->Join();
            delete args.fThread;
            delete fUpdateArgs.back().fUpdate;
            it = fUpdateArgs.erase(it);
            break;
         }
         ++it;
      }
   }
      
   if (interval == 0) {
      // update one
      UpdateCanvasImpl(pad);
   } else {
      if (fUpdateArgs.size() > 0) {
         printf("already registered. deregister pad by doing \"update\" once\n");
         return 1;
      }
      UpdateArg_t args;
      fUpdateArgs.push_back(args);
      fUpdateArgs.back().fPad = pad;
      fUpdateArgs.back().fInterval = interval;
      fUpdateArgs.back().fUpdate = new Bool_t;
      *fUpdateArgs.back().fUpdate = kTRUE;
      fUpdateArgs.back().fThread = new TThread(UpdateCanvasThread,&fUpdateArgs.back());
      fUpdateArgs.back().fThread->Run();
//      printf("not implimented yet for %s with %d\n",pad->GetName(),interval);
   }
   return 1;
}

void TCmdUpdateCanvas::Help()
{
   printf("update [interval] [name] \n");
   printf("  interval: Update interval in second for auto update if non zero.\n");
   printf("            Zero interval means that the pad will be updated once\n");
   printf("            To finish auto update, please update the conrresponding pad once,\n");
   printf("            e.g., do \"update pad\"\n");
   printf("  name    : name of pad to be updated\n");
}


void* TCmdUpdateCanvas::UpdateCanvasThread(void *updatearg)
{
   UpdateArg_t &arg = *(UpdateArg_t*) updatearg;
   while (*arg.fUpdate) {
      UpdateCanvasImpl(arg.fPad);
      gSystem->Sleep(arg.fInterval*1000.);
   }
   return NULL;
}

void TCmdUpdateCanvas::UpdateCanvasImpl(TVirtualPad *pad)
{
   if (pad->GetCanvas()) pad = (TVirtualPad*)pad->GetCanvas();
   pad->Modified();
   TList *primitives = pad->GetListOfPrimitives();
   if (!primitives) return;
   TObject *obj;
   TIter    next(primitives);
   while ((obj = next())) {
      if (obj->InheritsFrom(TPad::Class())) {
         ((TVirtualPad*)obj)->Modified();
         ((TVirtualPad*)obj)->Update();
      }
   }
   pad->Update();
}
