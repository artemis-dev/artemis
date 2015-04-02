/* $Id:$ */
/**
 * @file   TCatReadoutPad.h
 * @date   Created : Nov 30, 2013 20:30:06 JST
 *   Last Modified : Mar 30, 2014 01:14:08 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATREADOUTPAD_H
#define TCATREADOUTPAD_H

#include <TObject.h>
#include <TVector3.h>
namespace art {
   class TCatReadoutPad;
   
}

class art::TCatReadoutPad  : public TObject {

public:
   TCatReadoutPad();
   TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                  Double_t *vertexX, Double_t *vertexY,
                  Double_t side,
                  Int_t numNeighbors,Int_t *neighbor);
   TCatReadoutPad(const TCatReadoutPad& obj);
   virtual ~TCatReadoutPad();

   virtual void Clear(Option_t *opt = "");
   virtual void Copy(TObject& obj) const;

   Int_t GetID() { return fID; }
   Int_t GetNumPoints() { return fNumPoints; }
   Double_t X() { return fX; }
   Double_t Y() { return fY; }
   Double_t* VertexX() { return fVertexX; }
   Double_t* VertexY() { return fVertexY; }
   Int_t*    Neighbor() { return fNeighbor; }
   Bool_t   IsNeighbor(Int_t id) {
      for (Int_t i = 0; i!=fNumNeighbors; i++) {
         if (fNeighbor[i] == id) return kTRUE;
      }
      return kFALSE;
   }
   void CalculatePosition(const TVector3 &direction,
                          TCatReadoutPad*  pad,Double_t charge1, Double_t charge2,
                          Double_t &x, Double_t &z);
   
   static const Int_t kMaxNumPoints = 10;
   
protected:
   Int_t fID; // id of this pad
   Int_t fNumPoints; // number of points
   Int_t fNumNeighbors; // number of neighbors
   Double_t fSide; // length of side
   Double_t fX; // x position of center
   Double_t fY; // y position of center
   Double_t *fVertexX; //[fNumPoints]
   Double_t *fVertexY; //[fNumPoints]
   Int_t *fNeighbor; //[fNumNeighbors]

   ClassDef(TCatReadoutPad,1); // information on readout pad
};

#endif // end of #ifdef TCATREADOUTPAD_H
