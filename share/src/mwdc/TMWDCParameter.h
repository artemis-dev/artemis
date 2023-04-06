/**
 * @file   TMWDCParameter.h
 * @brief  mwdc parameter object
 *
 * @date   Created       : 2014-03-13 09:40:42 JST
 *         Last Modified : 2014-07-10 16:28:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_E8F391B6_AD3C_4E93_9B7E_9235CFD58F2A
#define INCLUDE_GUARD_UUID_E8F391B6_AD3C_4E93_9B7E_9235CFD58F2A

#include "TParameterObject.h"

namespace art {
   class TMWDCParameter;
   class TMWDCPlaneInfo;
}

class art::TMWDCParameter : public TParameterObject {
public:
   TMWDCParameter();
   virtual ~TMWDCParameter();

   TMWDCParameter(const TMWDCParameter& rhs);
   TMWDCParameter& operator=(const TMWDCParameter& rhs);

   void SetX(Double_t val) {fPosition[0] = val;}
   void SetY(Double_t val) {fPosition[1] = val;}
   void SetZ(Double_t val) {fPosition[2] = val;}
   void SetAngle(Double_t val) {fAngle = val;}
   void SetTurned(Bool_t val) {fIsTurned = val;}

   Double_t GetX() const {return fPosition[0];}
   Double_t GetY() const {return fPosition[1];}
   Double_t GetZ() const {return fPosition[2];}
   Double_t GetAngle() const {return fAngle;}

   Bool_t IsTurned() const {return fIsTurned;}

   void SetConfigurationFile(const TString& str) {fConfigurationFile = str;}
   TString GetConfigurationFile() const {return fConfigurationFile;}

   TMWDCPlaneInfo *GetPlane(const TString& name, Bool_t isLocal = kFALSE);
   const TMWDCPlaneInfo *GetPlane(const TString& name, Bool_t isLocal = kFALSE) const;

   const TList *GetPlaneList(Bool_t isLocal = kFALSE) const;

   void Copy(TObject &obj) const;
   void Print(Option_t*) const;

   Bool_t LoadYAMLNode(const YAML::Node&);
   void PrintDescriptionYAML() const;
   void OutputDescriptionYAML(std::ostream&) const;

protected:
   static const Int_t kPosDim = 3;
   Double_t fPosition[kPosDim]; //[kPosDim] Position in global coordinate
   Double_t fAngle;             // rotation angle around z axis (deg)
   Int_t    fIsTurned;          // 180 deg flipped by y axis if non zero
   TString  fConfigurationFile; //! filename of plane configuration

   TList   *fPlanes;            // list of TMWDCPlaneInfo in local coordinate
   TList   *fPlanesGlobal;      // list of TMWDCPlaneInfo in global coordinate

   void GenerateGlobal();

private:

   ClassDef(TMWDCParameter,1) // mwdc parameter object
};

#endif // INCLUDE_GUARD_UUID_E8F391B6_AD3C_4E93_9B7E_9235CFD58F2A
