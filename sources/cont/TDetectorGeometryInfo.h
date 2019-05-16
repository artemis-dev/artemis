/* @file TDetectorGeometryInfo.h
 * @brief contains detector position and dimension to produce a bin for TH2Poly
 *
 * @date Create        : 2019-05-16 11:31:09 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#ifndef INCLUDE_GUARD_UUID_7A7636E9_DC18_41A2_AAB1_3031B1815ADB
#define INCLUDE_GUARD_UUID_7A7636E9_DC18_41A2_AAB1_3031B1815ADB

#include "TNamed.h"
#include "IPosition.h"

namespace art {
   class TDetectorGeometryInfo;
}

class art::TDetectorGeometryInfo : public TNamed, public IPosition {
public:
   TDetectorGeometryInfo();
   TDetectorGeometryInfo(int id, double x, double y, double z);
   virtual ~TDetectorGeometryInfo();

   virtual void SetX(double x) { fPos[0] = x; }
   virtual void SetY(double y) { fPos[1] = y; }
   virtual void SetZ(double z) { fPos[2] = z; }
   virtual void SetID(int id) { fID = id; }

   virtual double X() const { return fPos[0]; }
   virtual double Y() const { return fPos[1]; }
   virtual double Z() const { return fPos[2]; }

   virtual int GetID() const { return fID; }

protected:
   Int_t fID;
//   Int_t fNumVertexPoints;
   Double_t fPos[3];
//   std::vector<Double_t> fVertexX;
//   std::vector<Double_t> fVertexY;
//   std::vector<Double_t> fVertexZ;
//
   
   
   ClassDef(TDetectorGeometryInfo,1)
}; // end of TDetectorGeometryInfo

#endif // #ifndef INCLUDE_GUARD_UUID_7A7636E9_DC18_41A2_AAB1_3031B1815ADB
