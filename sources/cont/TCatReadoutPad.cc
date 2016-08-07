/* $Id:$ */
/**
 * @file   TCatReadoutPad.cc
 * @date   Created : Nov 30, 2013 20:30:25 JST
 *   Last Modified : 2016-07-29 16:03:31 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatReadoutPad.h"
#include <TMath.h>
#include <TClonesArray.h>

ClassImp(art::TCatReadoutPad);

using art::TCatReadoutPad;

TCatReadoutPad::TCatReadoutPad()
   : fID(0), fNumPoints(0), fNumNeighbors(0),fX(0.), fY(0.),
     fVertexX(NULL), fVertexY(NULL), fNeighbor(NULL), fIntersection(NULL)
{
}

TCatReadoutPad::TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                               Double_t *vertexX, Double_t *vertexY,
                               Double_t side, Int_t numNeighbors, Int_t *neighbor)
   : fID(id), fNumPoints(numPoints), fNumNeighbors(numNeighbors), fSide(side),fX(x), fY(y),
     fVertexX(NULL), fVertexY(NULL), fNeighbor(NULL), fIntersection(NULL)
{
   fVertexX  = new Double_t[kMaxNumPoints];
   fVertexY  = new Double_t[kMaxNumPoints];
   fNeighbor = new Int_t[kMaxNumPoints];
   fIntersection = new TVector3[kMaxNumPoints];

   for (Int_t i=0; i!=fNumPoints; i++) {
      fVertexX[i] = vertexX[i];
      fVertexY[i] = vertexY[i];
      fNeighbor[i] = neighbor[i];
   }
}

TCatReadoutPad::TCatReadoutPad(const TCatReadoutPad& obj)
   : TObject(obj)
{
   fID        = obj.fID;
   fNumPoints = obj.fNumPoints;
   fX         = obj.fX;
   fY         = obj.fY;
   fVertexX  = new Double_t[kMaxNumPoints];
   fVertexY  = new Double_t[kMaxNumPoints];
   fNeighbor = new Int_t[kMaxNumPoints];
   fIntersection = new TVector3[kMaxNumPoints];

   for (Int_t i=0; i!=fNumPoints; i++) {
      fVertexX[i] = obj.fVertexX[i];
      fVertexY[i] = obj.fVertexY[i];
      fNeighbor[i] = obj.fNeighbor[i];
      fIntersection[i] = obj.fIntersection[i];
   }

      
}


TCatReadoutPad::~TCatReadoutPad()
{
   if (fVertexX) delete [] fVertexX;
   if (fVertexY) delete [] fVertexY;
   if (fNeighbor) delete [] fNeighbor;
   if (fIntersection) delete [] fIntersection;
}

void TCatReadoutPad::Clear(Option_t *opt)
{
   fNumPoints = 0;
   fID = 0;
   fX = 0.;
   fY =0.;
}

void TCatReadoutPad::Copy(TObject &obj) const
{
   TCatReadoutPad& pad = (TCatReadoutPad&)obj;
   pad.fID        = fID;
   pad.fNumPoints = fNumPoints;
   pad.fX         = fX;
   pad.fY         = fY;
   pad.fVertexX  = new Double_t[kMaxNumPoints];
   pad.fVertexY  = new Double_t[kMaxNumPoints];
   pad.fNeighbor = new Int_t[kMaxNumPoints];
   pad.fIntersection = new TVector3[kMaxNumPoints];
   

   for (Int_t i=0; i!=fNumPoints; i++) {
      pad.fVertexX[i] = fVertexX[i];
      pad.fVertexY[i] = fVertexY[i];
      pad.fNeighbor[i] = fNeighbor[i];
      pad.fIntersection[i] = fIntersection[i];
   }
}

void TCatReadoutPad::CalculatePosition(const TVector3 &direction,
                                       TCatReadoutPad *p2,
                                       Double_t q1, Double_t q2,
                                       Double_t &x, Double_t &z)
{
   TVector3 v1(fX,fY,0);
   TVector3 v2(p2->fX,p2->fY,0);
   TVector3 unit = (fSide/2.)*((v2-v1).Unit());
   Double_t angle = (v2-v1).DeltaPhi(direction)/TMath::Pi()*180.;
   angle = (angle < 0.) ? angle + 180.: angle;
//         unit.RotateY(90/180.*TMath::Pi());
   if (angle < 90.) {
      unit.RotateZ(90/180.*TMath::Pi());
   } else {
      unit.RotateZ(-90/180.*TMath::Pi());
   }
   TVector3 pos = 0.5*(v1+v2) + (q1-q2)/(q1+q2)*unit;
//   TVector3 pos = (q1-q2)/(q1+q2);
//   printf("(id1,id2) = (%d,%d)\n",fID,p2->fID);
//   unit.Print();
//   pos.Print();
   x = pos.X();
   z = pos.Y();
}

void TCatReadoutPad::GetIntersection(const TVector3 &a1, const TVector3 &a2, Int_t &num, TVector3 *&output) const
{
   const Double_t EPS = TMath::Limits<Double_t>::Epsilon();
   TVector3 va = a2 - a1;
   num = 0;
   for (Int_t iP = 0; iP != fNumPoints - 1; ++iP) {
      TVector3 b1(fVertexX[iP],0,fVertexY[iP]);
      TVector3 b2(fVertexX[iP+1],0,fVertexY[iP+1]);
      TVector3 vb = b2 - b1;

      if (!(va.Cross(b1-a1).Y() * va.Cross(b2-a1).Y() < EPS)  || 
          !(vb.Cross(a1-b1).Y() * vb.Cross(a2-b1).Y() < EPS)) {
         // two line segments do not cross each other
         continue;
      }
      
      Double_t d1 = TMath::Abs(vb.Cross(a1-b1).Y());
      Double_t d2 = TMath::Abs(vb.Cross(a2-b1).Y());
      Double_t t = d1/(d1+d2);
      TVector3 p = a1 + t * va;
      fIntersection[num] = p;
      ++num;
   }
   output = fIntersection;
}
