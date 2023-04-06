/**
 * @file   TP2PKinematicsData.cc
 * @brief
 *
 * @date   Created:       2013-12-11 11:18:16
 *         Last Modified: 2014-01-08 16:51:53
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TP2PKinematicsData.h"

#include "constant.h"

using art::sh04::TP2PKinematicsData;

ClassImp(art::sh04::TP2PKinematicsData)

// Default constructor
TP2PKinematicsData::TP2PKinematicsData() 
   : fRecoilL(NULL), fRecoilR(NULL),
     fIncident(kInvalidD,kInvalidD,kInvalidD,kInvalidD),
     fResidual(kInvalidD,kInvalidD,kInvalidD,kInvalidD),
     fSp(kInvalidD), fEx(kInvalidD) {
}

TP2PKinematicsData::~TP2PKinematicsData() {
}

// Copy constructor
TP2PKinematicsData::TP2PKinematicsData(const TP2PKinematicsData& rhs) 
   : TDataObject(rhs), fRecoilL(rhs.fRecoilL), fRecoilR(rhs.fRecoilR),
     fIncident(rhs.fIncident), fResidual(rhs.fResidual), fSp(rhs.fSp) {
}

// Assignment operator
TP2PKinematicsData& TP2PKinematicsData::operator=(const TP2PKinematicsData& rhs) {
   if (this != &rhs) {
      ((TP2PKinematicsData&)rhs).Copy(*this);
   }
   return *this;
}

void TP2PKinematicsData::Copy(TObject &obj) const {
   TDataObject::Copy(obj);
   TP2PKinematicsData &cobj = (TP2PKinematicsData&)obj;
   cobj.fRecoilL = fRecoilL;
   cobj.fRecoilR = fRecoilR;
   cobj.fIncident = fIncident;
   cobj.fResidual = fResidual;
   cobj.fSp = fSp;
}

void TP2PKinematicsData::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   fRecoilL = NULL;
   fRecoilR = NULL;
   fIncident.SetXYZT(kInvalidD,kInvalidD,kInvalidD,kInvalidD);
   fResidual.SetXYZT(kInvalidD,kInvalidD,kInvalidD,kInvalidD);
   fSp = kInvalidD;
}
