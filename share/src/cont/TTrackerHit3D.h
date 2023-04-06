/* $Id:$ */
/**
 * @file   TTrackerHit3D.h
 * @date   Created : Feb 10, 2014 17:10:07 JST
 *   Last Modified : Apr 21, 2014 17:38:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TTRACKERHIT3D_H
#define TTRACKERHIT3D_H

#include <TDataObject.h>
#include <TVector3.h>
#include <TRef.h>
#include <TCatPulseShape.h>

namespace art {
   class TTrackerHit3D;
   class TCatPulseShape;
}



class art::TTrackerHit3D  : public TDataObject {

public:
   TTrackerHit3D();
   ~TTrackerHit3D();

   virtual Float_t GetTime() const { return fTime; } 
   virtual const TVector3& GetPosition() const { return fPosition; }
   virtual Float_t GetCharge() const { return fCharge; }
   virtual Float_t GetDEDX() const { return fdEdX; }
   virtual Float_t GetOffsetDiff() const { return fOffsetDiff; }

   virtual Double_t X() const { return fPosition.X(); }
   virtual Double_t Y() const { return fPosition.Y(); }
   virtual Double_t Z() const { return fPosition.Z(); }
                        
   virtual void SetTime(Float_t time) { fTime = time; }
   virtual void SetPosition(Double_t x, Double_t y, Double_t z) { fPosition.SetXYZ(x,y,z); }
   virtual void SetCharge(Float_t charge) { fCharge = charge; }
   virtual void SetDEDX(Float_t dEdX) { fdEdX = dEdX; }
   virtual void SetOffsetDiff(Float_t diff) { fOffsetDiff = diff; }

   virtual TCatPulseShape* GetPulse1() { return (TCatPulseShape*)&fPulse1; }
   virtual TCatPulseShape* GetPulse2() { return (TCatPulseShape*)&fPulse2; }
   virtual void  SetPulses(TCatPulseShape *p1, TCatPulseShape *p2){
      p1->Copy(fPulse1);
      p2->Copy(fPulse2);
   }


protected:
   TVector3 fPosition;
   Float_t  fTime;
   Float_t  fOffsetDiff;
   Float_t  fCharge;
   Float_t  fdEdX;
   TCatPulseShape fPulse1; //
   TCatPulseShape fPulse2; //
   
   ClassDef(TTrackerHit3D,1);
};
#endif // end of #ifdef TTRACKERHIT3D_H
