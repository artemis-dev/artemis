/**
 * @file   TBeamZ.cc
 * @brief
 *
 * @date   Created:       2014-02-24 14:02:21
 *         Last Modified: 2014-02-24 14:23:04
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TBeamZ.h"

using art::sh04::TBeamZ;

ClassImp(art::sh04::TBeamZ)

// Default constructor
TBeamZ::TBeamZ()
   : fL(0), fR(0), fAverage(0){
}

TBeamZ::~TBeamZ() {
}

// // Copy constructor
// TBeamZ::TBeamZ(const TBeamZ& rhs) {
// }

// // Assignment operator
// TBeamZ& TBeamZ::operator=(const TBeamZ& rhs) {
//    if (this != &rhs) {
//       // TODO: implement copy
//    }
//    return *this;
// }
