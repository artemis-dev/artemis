/**
 * @file   TCoinRegData.cc
 * @brief
 *
 * @date   Created:       2014-01-15 14:22:23
 *         Last Modified: 2014-01-15 14:25:52
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TCoinRegData.h"
#include <algorithm>
using art::TCoinRegData;

ClassImp(TCoinRegData)

TCoinRegData::TCoinRegData() {
}

TCoinRegData::~TCoinRegData() {
}

// Copy constructor
TCoinRegData::TCoinRegData(const TCoinRegData& rhs) 
   : TDataObject(rhs) {
}

// Assignment operator
TCoinRegData& TCoinRegData::operator=(const TCoinRegData& rhs) {
   return static_cast<TCoinRegData&>(TDataObject::operator=(rhs));
}

