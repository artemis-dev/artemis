/** @class art::TCmdRg
 * @file   TCmdRg.cc
 * @brief  command for setting axis range
 *
 * @date   Created       : 2016-10-09 13:38:23 JST
 *         Last Modified : 2018-02-04 11:23:57 JST (ota)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TCmdRg.h"

#include <TString.h>
#include <TH1.h>
#include "TCatHistManager.h"

using art::TCmdRg;

ClassImp(TCmdRg)

TCmdRg::TCmdRg(const EAxis axis)
{
   const Char_t axisLabel = 'x' + (Char_t) axis;
   SetName(TString::Format("rg%c",axisLabel));
   SetTitle(TString::Format("set range of %c axis",axisLabel));

   switch (axis) {
   case kX:
      fGetAxis = &TH1::GetXaxis;
      break;
   case kY:
      fGetAxis = &TH1::GetYaxis;
      break;
   case kZ:
      fGetAxis = &TH1::GetZaxis;
      break;
   default:
      fGetAxis = NULL;
      break;
   }
}

TCmdRg::~TCmdRg()
{
}

void TCmdRg::Help()
{
   printf("usage: %s [hist_range] min max\n",GetName());
}

namespace {
   const Int_t NUM_ARGUMENT_WITH_RANGE = 3;
   const Int_t NUM_ARGUMENT_WITHOUT_RANGE = 2;
}

Long_t TCmdRg::Cmd(vector<TString> tokens)
{
   const Int_t n = tokens.size() - 1;
   TString  range;
   Double_t min(0.),max(0.);
   switch (n) {
      case NUM_ARGUMENT_WITH_RANGE:
	 range = tokens[1];
	 min = tokens[2].Atof();
	 max = tokens[3].Atof();
	 break;
      case NUM_ARGUMENT_WITHOUT_RANGE:
	 range = TCatCmd::kRangeDefault;
	 min = tokens[1].Atof();
	 max = tokens[2].Atof();
	 break;
      default:
	 Help();
	 return 1;
   }

   Int_t id1, id2;
   id1 = id2 = -1;
   GetRange(range,id1,id2);

   ++id2;
   for (Int_t id = id1; id != id2; ++id) {
      TH1 *const h = dynamic_cast<TH1*>(TCatHistManager::Instance()->GetObject(id));
      if (!h) {
	 printf("no such historgram with id = %d\n",id);
	 return 1;
      }
      Run(h,min,max);
   }

   return 1;
}

///////////////////////////////////////////////////////////////////
/// Execute command
/// @param[in] h        histogram whose viewing range will be changed
/// @param[in] ufirst   lower range of the viewing range
/// @param[in] ulast    upper range of the viewing range

Long_t TCmdRg::Run(TH1 *h, Double_t ufirst, Double_t ulast) const
{
   TAxis *const axis = (h->*fGetAxis)();
   if (axis) axis->SetRangeUser(ufirst,ulast);
   return 1;
}
