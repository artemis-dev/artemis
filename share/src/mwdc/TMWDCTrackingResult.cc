/**
 * @file   TMWDCTrackingResult.cc
 * @brief
 *
 * @date   Created:       2013-10-17 16:04:53
 *         Last Modified: 2018-07-13 22:31:34 (masuoka)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCTrackingResult.h"

#include <algorithm>
#include <iostream>

#include <TClass.h>

#include "constant.h"

using art::TMWDCTrackingResult;

ClassImp(art::TMWDCTrackingResult);

// Default constructor
TMWDCTrackingResult::TMWDCTrackingResult()
   : fNPlane(0), fNPlaneValid(0), fPlaneCombinationID(0),
     fNParameter(0), fNCombination(0), fNCombinationProcessed(0),
     fWireIDAdopted(0), fDriftLengthAdopted(0),
     fSSR(kInvalidD), fResidual(0),
     fSigma(kMAXNPARAMETER,kInvalidD), fVar(kMAXNPARAMETER,kInvalidD),
     fCI(kMAXNPARAMETER,kInvalidD), fTimeCost(kInvalidD), fPlaneName(kMAXNPARAMETER,"") {
   TDataObject::SetID(kDefault);
}

TMWDCTrackingResult::~TMWDCTrackingResult() {
}

// Copy constructor
TMWDCTrackingResult::TMWDCTrackingResult(const TMWDCTrackingResult& rhs)
   : TTrack(rhs),
     fNPlane(rhs.fNPlane), fNPlaneValid(rhs.fNPlaneValid),
     fPlaneCombinationID(rhs.fPlaneCombinationID),
     fNParameter(rhs.fNParameter),
     fNCombination(rhs.fNCombination), fNCombinationProcessed(rhs.fNCombinationProcessed),
     fWireIDAdopted(rhs.fWireIDAdopted), fDriftLengthAdopted(rhs.fDriftLengthAdopted),
     fSSR(rhs.fSSR), fResidual(rhs.fResidual),
     fSigma(rhs.fSigma), fVar(rhs.fVar), fCI(rhs.fCI), fTimeCost(rhs.fTimeCost), fPlaneName(rhs.fPlaneName) {
}

// Assignment operator
TMWDCTrackingResult& TMWDCTrackingResult::operator=(const TMWDCTrackingResult& rhs) {
   if (this != &rhs) {
      ((TMWDCTrackingResult&)rhs).Copy(*this);
   }
   return *this;
}

void TMWDCTrackingResult::Init(const Int_t nPlane) {
   if (fNPlane != nPlane) {
      fNPlane = nPlane;
      fWireIDAdopted.resize(fNPlane);
      fDriftLengthAdopted.resize(fNPlane);
      fResidual.resize(fNPlane);
      fPlaneName.resize(fNPlane);
   }
   Clear();
}

void TMWDCTrackingResult::Copy(TObject &obj) const {
   TTrack::Copy(obj);
   TMWDCTrackingResult &cobj = (TMWDCTrackingResult&)obj;
   if (fNPlane != cobj.fNPlane) {
      cobj.fNPlane = fNPlane;
      cobj.fWireIDAdopted.resize(fNPlane);
      cobj.fDriftLengthAdopted.resize(fNPlane);
      cobj.fResidual.resize(fNPlane);
   }

   cobj.fSSR = fSSR;

   cobj.fNPlaneValid = fNPlaneValid;
   cobj.fPlaneCombinationID = fPlaneCombinationID;
   cobj.fNParameter = fNParameter;
   cobj.fNCombination = fNCombination;
   cobj.fNCombinationProcessed = fNCombinationProcessed;
   cobj.fTimeCost = fTimeCost;

   std::copy(fResidual.begin(),fResidual.end(),cobj.fResidual.begin());
   std::copy(fWireIDAdopted.begin(),fWireIDAdopted.end(),cobj.fWireIDAdopted.begin());
   std::copy(fDriftLengthAdopted.begin(),fDriftLengthAdopted.end(),cobj.fDriftLengthAdopted.begin());
   std::copy(fSigma.begin(),fSigma.end(),cobj.fSigma.begin());
   std::copy(fVar.begin(),fVar.end(),cobj.fVar.begin());
   std::copy(fCI.begin(),fCI.end(),cobj.fCI.begin());
//   std::copy(fPlaneName.begin(),fPlaneName.end(),cobj.fPlaneName.begin());

   for (int i = 0; i < fPlaneName.size(); ++i) {
      cobj.fPlaneName.push_back(fPlaneName[i]);
      
   }
}

void TMWDCTrackingResult::Clear(Option_t *opt) {
   TTrack::Clear(opt);
   TDataObject::SetID(kDefault);
   fSSR = kInvalidD;
   fNPlaneValid = 0;
   fPlaneCombinationID = -1;
   fNParameter = 0;
   fNCombination = 0;
   fNCombinationProcessed = 0;
   fTimeCost = kInvalidD;

   std::fill(fResidual.begin(),fResidual.end(),kInvalidD);
   std::fill(fWireIDAdopted.begin(),fWireIDAdopted.end(),-1);
   std::fill(fDriftLengthAdopted.begin(),fDriftLengthAdopted.end(),kInvalidD);
   std::fill(fSigma.begin(),fSigma.end(),kInvalidD);
   std::fill(fVar.begin(),fVar.end(),kInvalidD);
   std::fill(fCI.begin(),fCI.end(),kInvalidD);
   std::fill(fPlaneName.begin(),fPlaneName.end(),"");
}

void TMWDCTrackingResult::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "TrackingID: " << GetID() << "\t"
	     << "nParameter: " << GetNParameter() << "\t"
	     << "SSR: " << GetSSR() << "\t"
	     << "TimeCost: " << GetTimeCost() << std::endl;
}

