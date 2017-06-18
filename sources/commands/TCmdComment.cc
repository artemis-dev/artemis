/**
 * @file   TCmdComment.cc
 * @brief  write comment in pad
 *
 * @date   Created       : 2017-06-18 03:08:46 JST
 *         Last Modified : 2017-06-18 03:08:47 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdComment.h"
#include <TPad.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TROOT.h>

using art::TCmdComment;

ClassImp(TCmdComment)

TCmdComment::TCmdComment()
{
   SetName("comment");
   SetTitle("write comment in pad");
}

TCmdComment::~TCmdComment()
{
}

TCmdComment* TCmdComment::Instance()
{
   static TCmdComment instance;
   return &instance;
}

void TCmdComment::Help()
{
   printf("usage: %s comment",GetName());
}

Long_t TCmdComment::Cmd(vector<TString> tokens)
{
   tokens.erase(tokens.begin());
   TString comment = "";
   for(vector<TString>::iterator it = tokens.begin(), end = tokens.end();
       it != end;
       ++it) {
      comment.Append(*it);
      comment.Append(" ");
   }
   return Run(comment);
}

Long_t TCmdComment::Run(const TString& comment)
{
   if (gPad == NULL) {
      return 1;
   }

   {
      TLatex *label;
      const TString label_name = TString::Format("comment_%s",
                                                 gPad->GetName());
      if ((label = dynamic_cast<TLatex*>(gROOT->FindObject(label_name)))) {
         delete label;
      }

      label = new TLatex(gStyle->GetPadLeftMargin(),
                         1-gStyle->GetPadTopMargin(),
                         comment);
      label->SetName(label_name);
      label->SetNDC();
      label->SetTextAlign(10);
      label->SetTextSize(0.03);
      label->Draw("same");
   }
   
   return 1;
}

