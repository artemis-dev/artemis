/**
 * @file   TMWDCPlaneInfo.cc
 * @brief  MWDC plane infomation class
 *
 * @date   Created:       2013-10-17 11:53:20
 *         Last Modified: 2015-10-19 10:42:32 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCPlaneInfo.h"

#include "constant.h"

#include <TROOT.h>
#include <TMath.h>

#include <yaml-cpp/yaml.h>

using art::TMWDCPlaneInfo;

ClassImp(art::TMWDCPlaneInfo);

// Default constructor
TMWDCPlaneInfo::TMWDCPlaneInfo()
   : fX(kInvalidD), fY(kInvalidD), fZ(kInvalidD),
     fNWire(kInvalidI), fCellSize(kInvalidD), fAngle(kInvalidD),
     fCenter(kInvalidD) {}

TMWDCPlaneInfo::~TMWDCPlaneInfo() {}

void TMWDCPlaneInfo::Print(Option_t*) const {
   const TString indent(' ',gROOT->GetDirLevel());
   printf("TMWDCPlaneInfo name='%s'\n",GetName());
   printf("%s Position: (%.2f, %.2f, %.2f)\n",indent.Data(),fX,fY,fZ);
   printf("%s NWire: %d, Center: %.2f, CellSize: %.2f\n",indent.Data(),fNWire,fCenter,fCellSize);
   printf("%s Angle: %.2f deg",indent.Data(),fAngle);
}

Bool_t TMWDCPlaneInfo::LoadYAMLNode(const YAML::Node& node) {
   try {
      fZ        = node["z"].as<Double_t>();
      fNWire    = node["nWire"].as<Int_t>();
      fCellSize = node["cellSize"].as<Double_t>();
      fAngle    = node["angle"].as<Double_t>()    ;
      fCenter   = node["center"].as<Double_t>()  ;
      fX = 0;
      fY = 0;
   } catch (YAML::Exception& e){
      Error("LoadYAMLNode","Error occurred!");
      fprintf(stderr,"%s\n",e.what());
      return kFALSE;
   }
   return kTRUE;
}
