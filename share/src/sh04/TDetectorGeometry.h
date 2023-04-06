/**
 * @file   TDetectorGeometry.h
 * @brief
 *
 * @date   Created:       2014-02-20 13:53:36
 *         Last Modified: 2014-02-27 18:19:42 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TDETECTORGEOMETRY_H
#define TDETECTORGEOMETRY_H

#include <TParameterObject.h>

namespace art {
   namespace sh04 {
      class TDetectorGeometry;
   }
}

namespace YAML{
   class Node;
}

class art::sh04::TDetectorGeometry : public TParameterObject {
public:
   // Default constructor
   TDetectorGeometry();
   virtual ~TDetectorGeometry();
   // // Copy constructor
   // TDetectorGeometry(const TDetectorGeometry& rhs);
   // // Assignment operator
   // TDetectorGeometry& operator=(const TDetectorGeometry& rhs);

   Double_t GetMWDCDistance(Bool_t isRight) const {
      return isRight ? fMWDCDistanceR : fMWDCDistanceL;
   }
   Double_t GetMWDCAngle   (Bool_t isRight) const {
      return isRight ? fMWDCAngleR : fMWDCAngleL;
   }
   Double_t GetMWDCYOffset (Bool_t isRight) const {
      return isRight ? fMWDCYOffsetR : fMWDCYOffsetL;
   }
   Double_t GetPlasticDistance(Bool_t isRight) const {
      return isRight ? fPlasticDistanceR : fPlasticDistanceL;
   }
   Double_t GetPlasticAngle   (Bool_t isRight) const {
      return isRight ? fPlasticAngleR : fPlasticAngleL;
   }

   void SetMWDCDistance(Bool_t isRight, Double_t val) {
      (isRight ? fMWDCDistanceR : fMWDCDistanceL) = val;
   }
   void SetMWDCAngle   (Bool_t isRight, Double_t val) {
      (isRight ? fMWDCAngleR : fMWDCAngleL) = val;
   }
   void SetMWDCYOffset (Bool_t isRight, Double_t val) {
      (isRight ? fMWDCYOffsetR : fMWDCYOffsetL) = val;
   }
   void SetPlasticDistance(Bool_t isRight, Double_t val) {
      (isRight ? fPlasticDistanceR : fPlasticDistanceL) = val;
   }
   void SetPlasticAngle   (Bool_t isRight, Double_t val) {
      (isRight ? fPlasticAngleR : fPlasticAngleL) = val;
   }

   void Print(Option_t*) const;

   Bool_t LoadYAMLNode(const YAML::Node&);
   void PrintDescriptionYAML() const;

private:
   Double_t fMWDCDistanceL;
   Double_t fMWDCDistanceR;
   Double_t fMWDCAngleL;
   Double_t fMWDCAngleR;
   Double_t fMWDCYOffsetL;
   Double_t fMWDCYOffsetR;
   Double_t fPlasticDistanceL;
   Double_t fPlasticDistanceR;
   Double_t fPlasticAngleL;
   Double_t fPlasticAngleR;

   void LoadSideNode(const YAML::Node&, Bool_t isRight);
   void LoadMWDCNode(const YAML::Node&, Bool_t isRight);
   void LoadPlasticNode(const YAML::Node&, Bool_t isRight);
   
   ClassDef(art::sh04::TDetectorGeometry,1) // SHARAQ04 detector geometry
};

#endif // TDETECTORGEOMETRY_H
