/**
 * @file   TVDCPlaneProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-18 13:01:05
 *         Last Modified: 2024-09-21 12:48:14 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include <algorithm>

#include "TVDCPlaneProcessor.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TTimingChargeData.h"
#include "TMWDCHitData.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"

#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>

using art::TVDCPlaneProcessor;

ClassImp(art::TVDCPlaneProcessor);

// Default constructor
  TVDCPlaneProcessor::TVDCPlaneProcessor()
: fPlaneOut(NULL), fNPlane(kInvalidI), fNData(NULL), fConverterArray(NULL)
{
  RegisterInputCollection("InputCollection","input",
		fInputColName,TString("mwdc1_raw"),
		&fMWDCIn,"TClonesArray","art::TTimingChargeData");
  RegisterProcessorParameter("MWDCName","name of mwdc",
		fMWDCName,TString("mwdc1"));
  const StringVec_t defConverterList(1,"dtdx");
  RegisterProcessorParameter("ConverterList","dt2dx conversion parameter",
		fConverterList,defConverterList);
  RegisterProcessorParameter("SortType","0: ID, 1: Timing, 2: Charge (Default), 3: DriftLength",
		fSortTypeInput, (Int_t)kCharge);
  RegisterProcessorParameter("MAXDL","max drift length",
		fMAXDL, 10);
  RegisterProcessorParameter("SortOrder","0: ascending, 1: descending (Default)",
		fSortOrderInput, 0);//(Int_t)kDESC);
  RegisterProcessorParameter("GR_OR_LAS","0: GR , 1:LAS",
		fGRLAS, 0);//(Int_t)kDESC);
}

TVDCPlaneProcessor::~TVDCPlaneProcessor()
{
  delete fConverterArray;
  if(fPlaneOut) {
	 for (Int_t i = 0; i != fNPlane ; ++i) delete fPlaneOut[i];
	 delete [] fPlaneOut;
  }
  delete [] fNData;
}

void TVDCPlaneProcessor::Init(TEventCollection *col)
{
ievt=0;
  LoadParameter(col);            // fNWireID, fNPlane are set
  if (!IsValid(fNPlane)) return; // fNPlane is not valid if the loading failed

  if (fNPlane == (Int_t)fConverterList.size()) {
	 TConverterUtil::SetConverterArrayFromList(&fConverterArray,fConverterList,col);
  } else {
	 Warning("Init","size of ConverterList is different from number of plane. ConverterList setting will be ignored.");
  }

  fPlaneOut = new TClonesArray*[fNPlane];

  fPlaneID.clear();
  fWireID.clear();
  fMaxDetID = DETID_ORIGIN - 1;

  for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
	 const TString &outputName = fOutputColName.at(iPlane);

	 fPlaneOut[iPlane] = new TClonesArray(TMWDCHitData::Class());
	 fPlaneOut[iPlane]->SetName(outputName);
	 col->Add(outputName,fPlaneOut[iPlane],fOutputIsTransparent);

	 const Int_t n = fNWireID[iPlane];
	 fPlaneID.insert(fPlaneID.end(),n,iPlane);
	 for(Int_t i = 0; i != n; ++i) {
		fWireID.push_back(i + WIREID_ORIGIN);
	 }

	 const TString &converterStr =
		fConverterArray ? TString::Format("(converter: %s)",fConverterList.at(iPlane).Data()) : "";
	 Info("Init","%s (id %d:%d) => %s %s",
		  fInputColName.Data(),
		  fMaxDetID + 1, fMaxDetID + n,
		  outputName.Data(), converterStr.Data());

	 fMaxDetID += n;
  }

  fNData = new Int_t[fNPlane];

  InitSortParameter();
}

void TVDCPlaneProcessor::InitSortParameter()
{
  switch (fSortTypeInput) {
  case kID:
	 fSortType = TMWDCHitData::kID;
	 break;
  case kTiming:
	 fSortType = TMWDCHitData::kTiming;
	 break;
  case kCharge:
	 fSortType = TMWDCHitData::kCharge;
	 break;
  case kDriftLength:
	 fSortType = TMWDCHitData::kDriftLength;
	 break;
  default:
	 SetStateError(TString::Format("Invalid SortType option: %d",fSortTypeInput));
  }

  switch (fSortOrderInput) {
  case kASC:
	 fSortOrder = TMWDCHitData::kASC;
	 break;
  case kDESC:
	 fSortOrder = TMWDCHitData::kDESC;
	 break;
  default:
	 SetStateError(TString::Format("Invalid SortOrder option: %d",fSortOrderInput));
  }
}

Bool_t TVDCPlaneProcessor::LoadParameter(TEventCollection *col)
{
  const TMWDCParameter *const mwdcprm =
	 reinterpret_cast<TMWDCParameter*>(col->GetInfo(fMWDCName));
  if (!mwdcprm) {
	 SetStateError(TString::Format("no parameter object for '%s' found.",fMWDCName.Data()));
	 return kFALSE;
  }

  const TList *const planeList = mwdcprm->GetPlaneList();

  TIter next(planeList);
  while (const TMWDCPlaneInfo *const prm = reinterpret_cast<TMWDCPlaneInfo*>(next())) {
	 const TString &outputName = TString::Format("%s_%s",fMWDCName.Data(),prm->GetName());
	 fOutputColName.push_back(outputName);
	 fNWireID.push_back(prm->GetNWire());
	 fCellSize.push_back(prm->GetCellSize() * 0.5);
  }

  fNPlane = fOutputColName.size();
  return kTRUE;
}

void TVDCPlaneProcessor::Process()
{
  ievt++;
  if(ievt%30000==0){
  //printf("plane processor ievt=%d\n",ievt);
  }
//  printf("vdc1\n");
  /* initialization */
  for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
	 fPlaneOut[iPlane]->Clear("C");
  }

  std::fill(fNData,fNData+fNPlane,0.);

  /* get input */
//  	 printf("(*fMWDCIn)->GetEntriesFast()=%d\n",(*fMWDCIn)->GetEntriesFast());
//	 if((*fMWDCIn)->GetEntriesFast()>400)return ;
  int old_id =-1000;
  for(Int_t iHit=0, nHit = (*fMWDCIn)->GetEntriesFast(); iHit!=nHit; ++iHit) {
	 const TTimingChargeData *const inData =
		static_cast<TTimingChargeData*>((*fMWDCIn)->At(iHit));
	 const Int_t detID = inData->GetDetID();
	 if (detID < DETID_ORIGIN || detID > fMaxDetID) continue;
	 const Int_t planeID =  fPlaneID[detID - DETID_ORIGIN];

	 /* output */
	 TMWDCHitData *const outData =
		static_cast<TMWDCHitData*>(fPlaneOut[planeID]->ConstructedAt(fNData[planeID]++));

		const Int_t wireID = fWireID[detID - DETID_ORIGIN];
		//if(wireID==8)printf("id=70000000000000000000000,nhit=%d\n",nHit);
		outData->SetDetID(wireID);
//		if(planeID==0)printf("planeiD=%d, wireID=%d\n",planeID,wireID);
	 old_id = detID;

	 {
//		Double_t timing = inData->GetTiming();
		double timing = inData->GetTiming();
		//printf("planeiD=%d, detID=%d, timing=%lf\n",planeID,detID,timing);	
//		printf("timing_old=%lf\n",timing);
//		if(timing>500000){
//		if(timing>524288/2.){
////				printf("timing=%lf\n",timing);
//				timing= timing-524288;
////				getchar();
//		}else if(timing<-400000){
////				printf("timing=%lf\n",timing);
//				timing= timing+524288;
//				//getchar();
//		}

//		if(abs(timing)>40000)printf("tooooooooooooooooooooooogloooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooong\n");

//		printf("planeiD=%d, detID=%d, timing=%lf\n",planeID,detID,timing);	
	//int timing2 = (-timing/10.);
	//四捨五入してみよう
	
//	double timing2 = (timing/10.);
//	int timing3 = (int)(timing2-0.5);
//	if(timing2-timing3>0.5)timing3=timing3+1;
		
//		outData->SetTiming((int)timing);
		outData->SetTiming(timing);
		outData->SetTimestamp(inData->GetTimestamp());
                
		outData->SetCharge(inData->GetCharge());

		//printf("plane =%d,timing=%lf,dl=%lf\n",timing,timing);
		if (fConverterArray) {
//		  printf("zasdfae\n");
		  //const Double_t driftLength = fConverterArray->at(planeID)->Convert(timing2);
//		  const Double_t driftLength = fConverterArray->at(planeID)->Convert((double)timing3);
		  Double_t driftLength;
//		  if(timing<360&&timing>100)driftLength = fConverterArray->at(planeID)->Convert((double)timing);
//		  if(timing<400&&timing>50){
//		  if(timing<-1200&&timing>-1500){//LAS
//		  if((fGRLAS==0&&timing<400&&timing>50)||(fGRLAS==1&&timing<1400&&timing>1000)){//LAS
//		  if((fGRLAS==0&&timing<1100&&timing>600)||(fGRLAS==1&&timing<1400&&timing>1000)){//LAS

/*		  if((fGRLAS==0&&timing<1100&&timing>0)||(fGRLAS==1&&timing<1400&&timing>1000)){//LAS
			 driftLength = fConverterArray->at(planeID)->Convert(-1.*(double)timing);
		  }else{
			 driftLength=-1000;
		  }
*/
		  driftLength = fConverterArray->at(planeID)->Convert(timing);
		  
//		printf("plane =%d,timing=%lf,dl=%lf\n",planeID,timing,driftLength);
//		  if(timing>359.||timing<81.){
//		  if(timing>369.||timing<81.||!(driftLength>0.&&driftLength<1.)){
//	if(!(driftLength>0.&&driftLength<1.)){
//		  if(timing>1500.||timing<1200.||!(driftLength>0.&&driftLength<1.)){
//		outData->SetDriftLength(-10000.);
//		  }else{
		  //   outData->SetDriftLength( driftLength * fCellSize[planeID]);
		  outData->SetDriftLength( driftLength * fMAXDL);
//		  }
		}
	 }
  }

 TMWDCHitData::SetSortType(fSortType);
  TMWDCHitData::SetSortOrder(fSortOrder);
  for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
//	 fPlaneOut[iPlane]->Sort();
  }
//  printf("vdc2\n");
//  printf("plane processor end\n");
}
