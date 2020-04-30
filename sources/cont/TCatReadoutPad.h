/* $Id:$ */
/**
 * @file   TCatReadoutPad.h
 * @date   Created : Nov 30, 2013 20:30:06 JST
 *   Last Modified : 2018-05-08 13:21:47 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATREADOUTPAD_H
#define TCATREADOUTPAD_H

#include <TObject.h>
#include <TVector3.h>
#include <TMath.h>
namespace art {
   class TCatReadoutPad;
   
}

class art::TCatReadoutPad  : public TObject {

public:
   typedef enum { kGood, kBad, kLowGain } EPadState_t;

   
   TCatReadoutPad();
   TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                  Double_t *vertexX, Double_t *vertexY,
                  Double_t side, Int_t numNeighbors, Int_t* neighbor);
   TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                  Double_t *vertexX, Double_t *vertexY,
                  Double_t side);
   TCatReadoutPad(const TCatReadoutPad& obj);
   virtual ~TCatReadoutPad();

   virtual void Clear(Option_t *opt = "");
   virtual void Copy(TObject& obj) const;

   Int_t GetID() const { return fID; }
   Int_t GetNumPoints() const { return fNumPoints; }
   Double_t Side() const { return fSide; }
   Double_t X() const { return fX; }
   Double_t Y() const { return fY; }
   const Double_t* VertexX() const { return &fVertexX[0]; }
   const Double_t* VertexY() const { return &fVertexY[0]; }
   const Int_t*    Neighbor() const { return &fNeighbors[0]; }
   Bool_t   IsNeighbor(Int_t id) const { return fIsNeighbor[id]; }
   Double_t Distance(const TCatReadoutPad &pad) const {
      return TMath::Sqrt((pad.fX - fX) * (pad.fX - fX) + (pad.fY - fY) * (pad.fY - fY));
   }
      

   virtual Bool_t IsInside(Double_t xp, Double_t yp) const {
      const Int_t &np = fNumPoints;
      const Double_t* x = &fVertexX[0];
      const Double_t* y = &fVertexY[0];
      Int_t i, j = np-1 ;
      Bool_t oddNodes = kFALSE;
      for (i=0; i<np; i++) {
         if ((y[i]<yp && y[j]>=yp) || (y[j]<yp && y[i]>=yp)) {
            if (x[i]+(yp-y[i])/(y[j]-y[i])*(x[j]-x[i])<xp) {
               oddNodes = !oddNodes;
            }
         }
         j=i;
      }
      return oddNodes;
   }
      

   void SetNeighbors(const std::vector<Int_t> &neighbors);
   
   virtual void GetIntersection(const TVector3 &a1, const TVector3 &a2, std::vector<TVector3>& output) const;
   Double_t GetTrackLength(const TVector3 &a1, const TVector3 &a2) const;
   Double_t GetRangeToEnd(const TVector3& start, const TVector3& end) const   ;

   EPadState_t GetStatus() const { return fStatus; }
   void SetStatus (EPadState_t state) { fStatus = state; }
   Bool_t IsGood() const { return (fStatus == kGood); }
   Bool_t IsBad() const { return (fStatus == kBad); }
   Bool_t IsLowGain() const{ return (fStatus == kLowGain); }
   
   void CalculatePosition(const TVector3 &direction,
                          TCatReadoutPad*  pad,Double_t charge1, Double_t charge2,
                          Double_t &x, Double_t &z);
   
   static const Int_t kMaxNumPoints = 10;

   static void ListOfStatus();
   
protected:
   Int_t fID; // id of this pad
   Int_t fNumPoints; // number of points
   Int_t fNumNeighbors; // number of neighbors
   Double_t fSide; // length of side
   Double_t fX; // x position of center
   Double_t fY; // y position of center
   std::vector<Double_t> fVertexX; //
   std::vector<Double_t> fVertexY; //
   std::vector<Int_t> fNeighbors; // list of id of neighboring pads
   std::vector<Int_t> fIsNeighbor; // if the id is neighbor
   EPadState_t fStatus; // status of channel

   ClassDef(TCatReadoutPad,1); // information on readout pad
};

#endif // end of #ifdef TCATREADOUTPAD_H
