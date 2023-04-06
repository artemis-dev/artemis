/**
 * @file   TExtractOutputProcessor.cc
 * @brief  extract output with single ID from a branch
 *
 * @date   Created       : 2015-05-19 12:00:41 JST
 *         Last Modified : 2015-05-19 12:10:20 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TExtractOutputProcessor.h"

using art::TExtractOutputProcessor;

ClassImp(TExtractOutputProcessor)

TExtractOutputProcessor::TExtractOutputProcessor()
{
   RegisterProcessorParameter("InputID","ID of input which will be extracted",fInputID,0);
   RegisterProcessorParameter("Output","name of output",fOutputColName,TString("extracted"));
}

TExtractOutputProcessor::~TExtractOutputProcessor()
{
}

void TExtractOutputProcessor::Init(TEventCollection* col)
{
   if (fInputID < 0) {
      SetStateError("InputID must be non-negative integer.");
      return;
   }
   fOutputColNames.clear();
   fOutputColNames.assign(fInputID,"");
   fOutputColNames.push_back(fOutputColName.Data());
   TSeparateOutputProcessor::Init(col);
}
