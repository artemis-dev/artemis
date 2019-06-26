/**
 * @file   TCmdHcol.cc
 * @brief  change the line color of histogram
 *
 * @date   Created       : 2018-02-13 15:04:11 JST
 *         Last Modified :
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#include "TCmdHcol.h"
#include <TCatHistManager.h>
#include "TH1.h"

using art::TCmdHcol;

ClassImp(TCmdHcol)

TCmdHcol::TCmdHcol()
{
   SetName("hcol");
   SetTitle("change line color of 1D Histogram");
}

TCmdHcol::~TCmdHcol()
{
}

Long_t TCmdHcol::Cmd(vector<TString> tokens)
{
   if (tokens.size()!=3) {
      Help();
      return -1;
   }
//   TH1* hist = TCatHistManager::Instance();
   Int_t id = tokens[1].Atoi();
   TString col = tokens[2];
   Int_t cid;
   if (col.IsDigit()) 
      cid = col.Atoi();
   else if (col.IsAlpha()){
      cid = (col == "black")  ? 1
         :  (col == "red")    ? 2
         :  (col == "green")  ? 3
         :  (col == "blue")   ? 4
         :  (col == "yellow") ? 5
         :  (col == "pink")   ? 6
         :  4 ;
   } else {
      printf("Please input only number or color name");
      Help();
      return -1;
   }

      
   
   Run(id,cid);
   return 1;
}

Long_t TCmdHcol::Run(Int_t id, Int_t cid)
{
   TObject *obj = TCatHistManager::Instance()->GetObject(id);
   if (!strcmp(obj->ClassName(),"TH1F") || !strcmp(obj->ClassName(),"TH1D")) {
   TH1 *hist = dynamic_cast<TH1*>(TCatHistManager::Instance()->GetObject(id));
   
   hist->SetLineColor(cid);
   TCatHistManager::Instance()->DrawCurrent();
//   obj->SetLineColor(col);   

   return 1;
   } else {
   printf("Input histogram should be 1D\n");
      return -1;
   }
   
}

void TCmdHcol::Help()
{
   printf( "Hcol \n");
   printf( "NAME \n");
   printf( "          hcol\n");
   printf( "SYNOPSIS \n");
   printf( "          hcol [Histogram ID] [Color ID]\n");
   printf( "          ([Color ID] can replace to the color name that:\n");
   printf( "          black, red, green, blue, yellow, pink\n");
   printf( "DESCRIPTION \n");
   printf( "          change the line color of 1D histogram\n");
}
