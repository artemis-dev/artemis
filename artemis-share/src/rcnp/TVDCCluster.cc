/**
 * @file   TVDCCluster.cc
 * @brief
 *
 * @date   Created:       2013-08-14 10:34:09
 *         Last Modified: 2014-01-08 12:56:45
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TVDCCluster.h"
#include "constant.h"

#include <iostream>
#include <TClass.h>

using art::TVDCCluster;

ClassImp(art::TVDCCluster)

Int_t TVDCCluster::fSortType  = TVDCCluster::kID;
Int_t TVDCCluster::fSortOrder = TVDCCluster::kDESC;

// Default constructor
TVDCCluster::TVDCCluster() 
   : fTiming(kInvalidD), fCharge(kInvalidD),Clust_ID(kInvalidD),valid_cluster2(kInvalidD),valid_cluster(kInvalidD),fClust_Size(kInvalidD),fHitID(0),fHitDL(0),fHitTiming(0),fHitLR(0){
   TDataObject::SetID(kInvalidI);
}

TVDCCluster::~TVDCCluster() {
}

// Copy constructor
TVDCCluster::TVDCCluster(const TVDCCluster& rhs)
   : TDataObject(rhs), fTiming(rhs.fTiming), fCharge(rhs.fCharge) {
}
void TVDCCluster::Print(Option_t*) const {
/*  std::cout << "OBJ: " << IsA()->GetName() << "\t"
	 << "TrackingID: " << GetID() << "\t"
	 << "nParameter: " << GetNParameter() << "\t"
	 << "SSR: " << GetSSR() << "\t"
	 << "TimeCost: " << GetTimeCost() << std::endl;
	 */
  printf("aaa\n");
}

void TVDCCluster::Init() {


	 fHitID.resize(20);
	 fHitDL.resize(20);
	 fHitTiming.resize(20);
	 fHitLR.resize(20);
  Clear();
}
void TVDCCluster::Init2(int size) {


	 fHitID.resize(size);
	 fHitDL.resize(size);
	 fHitTiming.resize(size);
	 fHitLR.resize(size);
  Clear();
}
// Assignment operator
/*TVDCCluster& TVDCCluster::operator=(const TVDCCluster& rhs) {
   if (this != &rhs) {
      ((TVDCCluster&)rhs).Copy(*this);
   }
   return *this;
}
*/
/*

void TVDCCluster::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   this->SetTiming(kInvalidD);
   this->SetCharge(kInvalidD);
}
*/
