/**
 * @file   TMWDCPlaneInfo.h
 * @brief  MWDC plane infomation class
 *
 * @date   Created:       2013-10-17 11:40:04
 *         Last Modified: 2014-06-29 06:49:53 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMWDCPLANEINFO_H
#define TMWDCPLANEINFO_H

#include "TParameterObject.h"
#include <TString.h>
#include <TMath.h>

namespace art{
   class TMWDCPlaneInfo;
   class TMWDCParameter;
}

namespace YAML{
   class Node;
}

class art::TMWDCPlaneInfo : public TParameterObject {

   friend class TMWDCParameter;

public:
   // Default constructor
   TMWDCPlaneInfo();
   virtual ~TMWDCPlaneInfo();

   virtual void SetX(Double_t val) { fX = val; }
   virtual void SetY(Double_t val) { fY = val; }
   virtual void SetZ(Double_t val) { fZ = val; }
   virtual Double_t GetX() const { return fX; }
   virtual Double_t GetY() const { return fY; }
   virtual Double_t GetZ() const { return fZ; }

   virtual void SetNWire(Int_t val) { fNWire = val; }
   virtual Int_t GetNWire() const { return fNWire; }

   virtual void SetCellSize(Double_t val) { fCellSize = val; }
   virtual Double_t GetCellSize() const { return fCellSize; }

   virtual void SetAngle(Double_t val) { fAngle = val; }
   virtual Double_t GetAngle() const { return fAngle; }

   virtual void SetCenter(Double_t val) { fCenter = val; }
   virtual Double_t GetCenter() const { return fCenter; }

   virtual Double_t Cos() const { return TMath::Cos( fAngle * TMath::DegToRad()); };
   virtual Double_t Sin() const { return TMath::Sin( fAngle * TMath::DegToRad()); };

   virtual void Print(Option_t *opt = "") const;

protected:
   Double_t fX;         // X position of the plane in MWDC local coordinate (mm)
   Double_t fY;         // Y position of the plane in MWDC local coordinate (mm)
   Double_t fZ;         // Z position of the plane in MWDC local coordinate (mm)
   Int_t    fNWire;     // number of wires in the plane
   Double_t fCellSize;  // cell size (mm)
   Double_t fAngle;     // wire direction angle (deg)
   Double_t fCenter;    // central wire ID (don't have to be integer)

   virtual Bool_t LoadYAMLNode(const YAML::Node& node);

   ClassDef(TMWDCPlaneInfo,1) // container for MWDC plane information
};

#endif // TMWDCPLANEINFO_H
