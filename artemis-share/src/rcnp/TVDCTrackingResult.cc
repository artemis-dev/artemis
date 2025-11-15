/**
 * @file   TVDCTrackingResult.cc
 * @brief
 *
 * @date   Created:       2013-10-17 16:04:53
 *         Last Modified: 2014-01-17 16:54:07
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TVDCTrackingResult.h"

#include <algorithm>
#include <iostream>

#include <TClass.h>

#include "TTrack.h"
#include "constant.h"

using art::TVDCTrackingResult;

ClassImp(art::TVDCTrackingResult);

// Default constructor
TVDCTrackingResult::TVDCTrackingResult() 
  : fNPlane(0), fNPlaneValid(0), fPlaneCombinationID(0),
  fNParameter(0), fNCombination(0), fNCombinationProcessed(0),
  fWireIDAdopted(0), fDriftLengthAdopted(0),fLR(0),
  fSSR(kInvalidD),fSSR2(kInvalidD), fResidual(0),  fUSV(kInvalidD), 
  fSigma(kMAXNPARAMETER,kInvalidD), fCI(kMAXNPARAMETER,kInvalidD), fTimeCost(kInvalidD) {
	 TDataObject::SetID(kDefault);
  }

TVDCTrackingResult::~TVDCTrackingResult() {
}

// Copy constructor
TVDCTrackingResult::TVDCTrackingResult(const TVDCTrackingResult& rhs) 
  : TDataObject(rhs), fTrack(rhs.fTrack), fTrack2(rhs.fTrack2), 
  fNPlane(rhs.fNPlane), fNPlaneValid(rhs.fNPlaneValid), 
  fPlaneCombinationID(rhs.fPlaneCombinationID),
  fNParameter(rhs.fNParameter),
  fNCombination(rhs.fNCombination), fNCombinationProcessed(rhs.fNCombinationProcessed),
  fWireIDAdopted(rhs.fWireIDAdopted), fDriftLengthAdopted(rhs.fDriftLengthAdopted),fLR(rhs.fLR),

  fSSR(rhs.fSSR),fSSR2(rhs.fSSR2), fResidual(rhs.fResidual),
  fUSV(rhs.fUSV), fSigma(rhs.fSigma), fCI(rhs.fCI), fTimeCost(rhs.fTimeCost) {
  }

// Assignment operator
TVDCTrackingResult& TVDCTrackingResult::operator=(const TVDCTrackingResult& rhs) {
  if (this != &rhs) {
	 ((TVDCTrackingResult&)rhs).Copy(*this);
  }
  return *this;
}

void TVDCTrackingResult::Init(const Int_t nPlane) {
  if (fNPlane != nPlane) {
	 fNPlane = nPlane;
	 fWireIDAdopted.resize(fNPlane);
	 fDriftLengthAdopted.resize(fNPlane);
	 fLR.resize(fNPlane);
	 fResidual.resize(fNPlane);
  }
  Clear();
}

void TVDCTrackingResult::Copy(TObject &obj) const {
  TDataObject::Copy(obj);
  TVDCTrackingResult &cobj = (TVDCTrackingResult&)obj;
  if (fNPlane != cobj.fNPlane) {
	 cobj.fNPlane = fNPlane;
	 cobj.fWireIDAdopted.resize(fNPlane);
	 cobj.fDriftLengthAdopted.resize(fNPlane);
	 cobj.fLR.resize(fNPlane);
	 cobj.fResidual.resize(fNPlane);
  }

  cobj.fSSR = fSSR;
  cobj.fSSR2 = fSSR2;
  cobj.fTrack = fTrack;
  cobj.fTrack2 = fTrack2;

  cobj.fNPlaneValid = fNPlaneValid;
  cobj.fPlaneCombinationID = fPlaneCombinationID;
  cobj.fNParameter = fNParameter;
  cobj.fNCombination = fNCombination;
  cobj.fNCombinationProcessed = fNCombinationProcessed;
  cobj.fUSV = fUSV;
  cobj.fTimeCost = fTimeCost;

  std::copy(fResidual.begin(),fResidual.end(),cobj.fResidual.begin());
  std::copy(fWireIDAdopted.begin(),fWireIDAdopted.end(),cobj.fWireIDAdopted.begin());
  std::copy(fDriftLengthAdopted.begin(),fDriftLengthAdopted.end(),cobj.fDriftLengthAdopted.begin());
  std::copy(fLR.begin(),fLR.end(),cobj.fLR.begin());
  std::copy(fSigma.begin(),fSigma.end(),cobj.fSigma.begin());
  std::copy(fCI.begin(),fCI.end(),cobj.fCI.begin());
}

void TVDCTrackingResult::Clear(Option_t *opt) {
  TDataObject::Clear(opt);
  TDataObject::SetID(kDefault);
  fSSR = kInvalidD;
  fSSR2 = kInvalidD;
  fTrack.Clear(opt);
  fTrack2.Clear(opt);
  fNPlaneValid = 0;
  fPlaneCombinationID = -1;
  fNParameter = 0;
  fNCombination = 0;
  fNCombinationProcessed = 0;
  fUSV = kInvalidD;
  fTimeCost = kInvalidD;

  std::fill(fResidual.begin(),fResidual.end(),kInvalidD);
  std::fill(fWireIDAdopted.begin(),fWireIDAdopted.end(),-1);
  std::fill(fDriftLengthAdopted.begin(),fDriftLengthAdopted.end(),kInvalidD);
  std::fill(fLR.begin(),fLR.end(),-1);
  std::fill(fSigma.begin(),fSigma.end(),kInvalidD);
  std::fill(fCI.begin(),fCI.end(),kInvalidD);
  for(int i=0;i<4;i++){
	 for(int j=0;j<10;j++){
		VDC_res[i][j]=-1000.;
		VDC_apos[i][j]=-1000.;
		VDC_lr[i][j]=-1000.;
		VDC_adl[i][j]=-1000.;
////////		VDC_aid[i][j]=-1000.;
		CLST_pos[i][j]=-1000.;
		CLST_grad[i][j]=-1000.;
		for(int jk =0;jk<20;jk++){
		  CLST_dl[i][j][jk]=-1000.;
		  CLST_timing[i][j][jk]=-1000.;
		  CLST_id[i][j][jk]=-1000;
		}
		CLST_size[i][j]=-1000;
		CLST_sippai[i][j]=-1000;
		CLST_status[i][j]=-1000;
//		CLST_rsize[i][j]=-1000;
	 }
		chi2_plane[i]=-1000.;
		CLST_asize[i]=-1000.;
		CLST_osize[i]=-1000.;
	 CLST_num[i]=0;
	 VDC_nc[i]=0;
	 VDC_hit[i]=-1000.;

  }
  sumres=-1000.;
  dispres=-1000.;
}

void TVDCTrackingResult::Print(Option_t*) const {
  std::cout << "OBJ: " << IsA()->GetName() << "\t"
	 << "TrackingID: " << GetID() << "\t"
	 << "nParameter: " << GetNParameter() << "\t"
	 << "SSR: " << GetSSR() << "\t"
	 << "TimeCost: " << GetTimeCost() << std::endl;
}

