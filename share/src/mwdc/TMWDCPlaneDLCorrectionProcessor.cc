/**
 * @file   TMWDCPlaneDLCorrectionProcessor.cc
 * @brief  MWDC Apply for the drift length correction parameter created by art::TMWDCDriftLengthCorrectionProcessor
 *
 * @date   Created       : 2018-07-17 18:20:01 JST
 *         Last Modified : 2019-11-24 03:55:45 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#include "TMWDCPlaneDLCorrectionProcessor.h"
#include "TClonesArray.h"
#include "TMWDCHitData.h"
#include "TConverterUtil.h"
#include "constant.h"
#include "TMath.h"
#include "TConverterBase.h"

using art::TMWDCPlaneDLCorrectionProcessor;

namespace {
   const TString DEFAULT_CONV_NAME("no_conversion");
}

ClassImp(TMWDCPlaneDLCorrectionProcessor)

TMWDCPlaneDLCorrectionProcessor::TMWDCPlaneDLCorrectionProcessor()
  : fConverterArray(NULL)
{
   RegisterInputCollection("InputCollection", "input",
                           fInputColName, TString("dcx1_x1"),
                           &fPlaneIn, "TClonesArray", "art::TMWDCHitData");
   const StringVec_t defConverterList(1,"dldl");
   RegisterProcessorParameter("ConverterList", "dl correction parameter",
                              fConverterList, defConverterList);
   const Double_t defCellSize = 9.;
   RegisterProcessorParameter("CellSize","cell size",
                              fCellSize, defCellSize);
   RegisterProcessorParameter("Verbose", "verbose level (1: non quiet)",
                              fVerbose, 0);
                              
}

TMWDCPlaneDLCorrectionProcessor::~TMWDCPlaneDLCorrectionProcessor()
{
 delete fConverterArray;
}

void TMWDCPlaneDLCorrectionProcessor::Init(TEventCollection *col)
{
   if (fPlaneIn) {
      Info("Init","plane input %s Done",fInputColName.Data());
   }

   TConverterUtil::SetConverterArrayFromList(&fConverterArray,fConverterList,col);

   if (!fConverterArray) {
      SetStateError("no converter registered.");
      return;
   }
   
   
}

void TMWDCPlaneDLCorrectionProcessor::Process()
{
   for(Int_t iHit = 0; iHit < (*fPlaneIn)->GetEntriesFast(); ++iHit) {
      if (fVerbose) {
         printf("%s plane hit %d/%d pointer will create\n",fInputColName.Data(),iHit,(*fPlaneIn)->GetEntriesFast());
      }
      TMWDCHitData *const data = static_cast<TMWDCHitData*>((*fPlaneIn)->UncheckedAt(iHit));
      if (fVerbose) {
         printf("%s plane hit %d/%d pointer %p created\n",fInputColName.Data(),iHit,(*fPlaneIn)->GetEntriesFast(),data);
      }

      const Double_t driftLength = data->GetDriftLength();

      if (fConverterArray->empty()) {
         printf("Converter array size is 0\n");
         SetStateError("Converter array vector has no address\n");
         data->SetDriftLength(kInvalidD);
         break;
      }
      if (TMath::IsNaN(driftLength)) {
            data->SetDriftLength(kInvalidD);
            break;
         }
      const Double_t driftLengthCorrect = fConverterArray->at(0)->Convert(driftLength);
      if (fVerbose) printf("hoge\n");
      data->SetDriftLength(driftLengthCorrect * fCellSize * 0.5);
   }
}
