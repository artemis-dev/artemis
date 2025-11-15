/**
 * @file   TCatTrackResultPulse.h
 * @brief  track result pulse
 *
 * @date   Created       : 2015-06-10 21:17:42 JST
 *         Last Modified : 2016-10-19 15:52:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_9B268CFA_536C_4CD7_A8E5_26D9B830041C
#define INCLUDE_GUARD_UUID_9B268CFA_536C_4CD7_A8E5_26D9B830041C

#include "TCatPulseShape.h"
#include <TVector3.h>

namespace art {
   class TCatTrackResultPulse;
}


class art::TCatTrackResultPulse : public TCatPulseShape {
public:
   TCatTrackResultPulse();
   virtual ~TCatTrackResultPulse();

   //TCatTrackResultPulse(const TCatTrackResultPulse& rhs);
   //TCatTrackResultPulse& operator=(const TCatTrackResultPulse& rhs);

   void AddVertex(const TVector3& vec) {
      fVertex.push_back(vec);
      fNumVertex++;
   }
   const TVector3* GetVertex(Int_t i) const { return &fVertex[i]; }

   virtual void Clear(Option_t *opt = "") {
      TCatPulseShape::Clear(opt);
      fVertex.clear();
      fNumVertex = 0;
   }

   Int_t GetNumVertex() { return fNumVertex; }

   void SetTrackLength(Double_t length) { fTrackLength = length; }
   void SetEnergyDeposit(Double_t energy) { fEnergyDeposit = energy; }
   void SetDEDX(Double_t dedx) { fDEDX = dedx; }
   void SetAngle(Double_t angle) { fAngle = angle; }
   void SetDistance(Double_t distance) { fDistance = distance; }
   void SetRange(Double_t range) { fRange = range; }
   Double_t GetTrackLength() const { return fTrackLength; }
   Double_t GetEnergyDeposit() const { return fEnergyDeposit; }
   Double_t GetDEDX() const { return fDEDX; }
   Double_t GetRange() const { return fRange; }

protected:
   Int_t     fNumVertex; // number of vertex
   Double_t  fTrackLength; // length of track
   Double_t  fEnergyDeposit; // energy deposit (eV)
   Double_t  fDEDX; // dE/dX estimated
   Double_t   fAngle; // angle
   Double_t   fDistance; // distance to line
   Double_t   fRange; // range to the end point
   
   std::vector<TVector3> fVertex; //
   
   
private:

   ClassDef(TCatTrackResultPulse,1) // track result pulse
};

#endif // INCLUDE_GUARD_UUID_9B268CFA_536C_4CD7_A8E5_26D9B830041C

