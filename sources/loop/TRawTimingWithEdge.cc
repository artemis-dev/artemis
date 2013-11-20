/**
 * @file   TRawTimingWithEdge.cc
 * @brief
 *
 * @date   Created:       2013-08-13 13:04:27
 *         Last Modified: 2013-11-12 15:27:01
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TRawTimingWithEdge.h"

using art::TRawTimingWithEdge;

ClassImp(art::TRawTimingWithEdge)

// Default constructor
TRawTimingWithEdge::TRawTimingWithEdge() 
   : fIsLeading(true){
}

TRawTimingWithEdge::~TRawTimingWithEdge() {
}


