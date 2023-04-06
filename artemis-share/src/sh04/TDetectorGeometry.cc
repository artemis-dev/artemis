/**
 * @file   TDetectorGeometry.cc
 * @brief
 *
 * @date   Created:       2014-02-20 14:31:10
 *         Last Modified: 2014-02-27 18:32:28 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TDetectorGeometry.h"
#include <yaml-cpp/yaml.h>
#include <TClass.h>

using art::sh04::TDetectorGeometry;

ClassImp(TDetectorGeometry)

// Default constructor
TDetectorGeometry::TDetectorGeometry()
   : fMWDCDistanceL(0), fMWDCDistanceR(0),
     fMWDCAngleL(0), fMWDCAngleR(0),
     fMWDCYOffsetL(0), fMWDCYOffsetR(0),
     fPlasticDistanceL(0), fPlasticDistanceR(0),
     fPlasticAngleL(0), fPlasticAngleR(0) {
}

TDetectorGeometry::~TDetectorGeometry() {
}

// // Copy constructor
// TDetectorGeometry::TDetectorGeometry(const TDetectorGeometry& rhs) {
// }

// // Assignment operator
// TDetectorGeometry& TDetectorGeometry::operator=(const TDetectorGeometry& rhs) {
//    if (this != &rhs) {
//       // TODO: implement copy
//    }
//    return *this;
// }

void TDetectorGeometry::Print(Option_t*) const {
   printf("OBJ: %s\t%s\n",
	  IsA()->GetName(), GetName());
   printf("\tMWDCL:    angle = %.1lf deg.,\tdistance = %.1lf mm,\tyOffset = %.1lf mm\n",
	  GetMWDCAngle(0),GetMWDCDistance(0),GetMWDCYOffset(0));
   printf("\tPlasticL: angle = %.1lf deg.,\tdistance = %.1lf mm\n",
	  GetPlasticAngle(0),GetPlasticDistance(0));
   printf("\tMWDCR:    angle = %.1lf deg.,\tdistance = %.1lf mm,\tyOffset = %.1lf mm\n",
	  GetMWDCAngle(1),GetMWDCDistance(1),GetMWDCYOffset(1));
   printf("\tPlasticR: angle = %.1lf deg.,\tdistance = %.1lf mm\n",
	  GetPlasticAngle(1),GetPlasticDistance(1));
}

Bool_t TDetectorGeometry::LoadYAMLNode(const YAML::Node& node) {
   LoadSideNode(node["L"], kFALSE);
   LoadSideNode(node["R"], kTRUE);
   return kTRUE;
}

void TDetectorGeometry::LoadSideNode(const YAML::Node& node, Bool_t isRight) {
   LoadMWDCNode(node["MWDC"], isRight);
   LoadPlasticNode(node["Plastic"], isRight);
}

void TDetectorGeometry::LoadMWDCNode(const YAML::Node& node, Bool_t isRight) {
   if (isRight) {
      fMWDCDistanceR = node["distance"].as<Double_t>();
      fMWDCAngleR    = node["angle"].as<Double_t>();
      if (node["yOffset"]) fMWDCYOffsetR = node["yOffset"].as<Double_t>();
   } else {
      fMWDCDistanceL = node["distance"].as<Double_t>();
      fMWDCAngleL    = node["angle"].as<Double_t>();
      if (node["yOffset"]) fMWDCYOffsetL = node["yOffset"].as<Double_t>();
   }
   // node["distance"] >> (isRight ? fMWDCDistanceR : fMWDCDistanceL);
   // node["angle"] >> (isRight ? fMWDCAngleR : fMWDCAngleL);
//   if(const YAML::Node *subnode = node.FindValue("yOffset")) {
//      *subnode >> (isRight ? fMWDCYOffsetR : fMWDCYOffsetL);
//   }
}

void TDetectorGeometry::LoadPlasticNode(const YAML::Node& node, Bool_t isRight) {
   if (isRight) {
      fPlasticAngleR = node["angle"].as<Double_t>();
      fPlasticDistanceR = node["distance"].as<Double_t>();
   } else {
      fPlasticAngleL = node["angle"].as<Double_t>();
      fPlasticDistanceL = node["distance"].as<Double_t>();
   }
//   node["distance"] >> (isRight ? fPlasticDistanceR : fPlasticDistanceL);
//   node["angle"] >> (isRight ? fPlasticAngleR : fPlasticAngleL);
}

void TDetectorGeometry::PrintDescriptionYAML() const {}
