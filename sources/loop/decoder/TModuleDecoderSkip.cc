/**
 * @file   TModuleDecoderSkip.cc
 * @brief
 *
 * @date   Created:       2013-07-24 17:33:07
 *         Last Modified: 2013-07-31 09:57:06
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderSkip.h"

#include <TObjArray.h>

using art::TModuleDecoderSkip;

// Default constructor
TModuleDecoderSkip::TModuleDecoderSkip(const Int_t id)
   : TModuleDecoder(id, NULL) {
   printf("[TModuleDecorderSkip] ModuleID = %d will be skipped.\n",id);
}

// Default destructor
TModuleDecoderSkip::~TModuleDecoderSkip() {
}

