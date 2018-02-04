/* $Id:$ */
/**
 * @file   TCatReadoutPad.cc
 * @date   Created : Nov 30, 2013 20:30:25 JST
 *   Last Modified : 2016-11-12 01:21:38 JST (ota)
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
   : fID(0), fNumPoints(0), fNumNeighbors(0),fX(0.), fY(0.), fStatus(kGood)
{
}

TCatReadoutPad::TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                               Double_t *vertexX, Double_t *vertexY,
                               Double_t side, Int_t numNeighbors, Int_t *neighbor)
   : fID(id), fNumPoints(numPoints), fNumNeighbors(numNeighbors), fSide(side),fX(x), fY(y), fStatus(kGood)
{
   for (Int_t i = 0; i < numPoints; ++i) {
      fVertexX.push_back(vertexX[i]);
      fVertexY.push_back(vertexY[i]);
   }
   for (Int_t i = 0; i < numNeighbors; ++i) {
      fNeighbors.push_back(neighbor[i]);
   }

}

TCatReadoutPad::TCatReadoutPad(Int_t id, Int_t numPoints, Double_t x, Double_t y,
                               Double_t *vertexX, Double_t *vertexY,
                               Double_t side)
   : fID(id), fNumPoints(numPoints),fSide(side),fX(x), fY(y)
{
   for (Int_t i = 0; i < numPoints; ++i) {
      fVertexX.push_back(vertexX[i]);
      fVertexY.push_back(vertexY[i]);
   }
}


TCatReadoutPad::TCatReadoutPad(const TCatReadoutPad& obj)
   : TObject(obj)
{
   fID        = obj.fID;
   fNumPoints = obj.fNumPoints;
   fX         = obj.fX;
   fY         = obj.fY;
   fVertexX.resize(obj.fVertexX.size());
   fVertexY.resize(obj.fVertexY.size());
   fNeighbors.resize(obj.fNeighbors.size());

   for (Int_t i=0; i!=fNumPoints; i++) {
      fVertexX[i] = obj.fVertexX[i];
      fVertexY[i] = obj.fVertexY[i];
      fNeighbors[i] = obj.fNeighbors[i];
   }
}


TCatReadoutPad::~TCatReadoutPad()
{
}

void TCatReadoutPad::Clear(Option_t *opt)
{
   fNumPoints = 0;
   fID = 0;
   fX = 0.;
   fY =0.;
   fVertexX.clear();
   fVertexY.clear();
   fNeighbors.clear();
}

void TCatReadoutPad::Copy(TObject &obj) const
{
   TCatReadoutPad& pad = (TCatReadoutPad&)obj;
   pad.fID        = fID;
   pad.fNumPoints = fNumPoints;
   pad.fX         = fX;
   pad.fY         = fY;
   pad.fVertexX.resize(fVertexX.size());
   pad.fVertexY.resize(fVertexY.size());
   pad.fNeighbors.resize(fNeighbors.size());
   
   for (Int_t i=0; i!=fNumPoints; i++) {
      pad.fVertexX[i] = fVertexX[i];
      pad.fVertexY[i] = fVertexY[i];
      pad.fNeighbors[i] = fNeighbors[i];
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

void TCatReadoutPad::GetIntersection(const TVector3 &a1, const TVector3 &a2, std::vector<TVector3>& output) const
{
   const Double_t EPS = TMath::Limits<Double_t>::Epsilon();
   TVector3 va = a2 - a1;
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
      output.push_back(p);
   }
}

Double_t TCatReadoutPad::GetTrackLength(const TVector3& a1, const TVector3 &a2) const
{
   std::vector<TVector3> intersection;
   Int_t num;
   Double_t trackLength = 0.;
   GetIntersection(a1,a2,intersection);
   num = intersection.size();
   switch (num) {
   case 2:
      trackLength = (intersection[0] - intersection[1]).Mag();
      break;
   case 1:
      if (IsInside(a1.X(),a1.Z())) {
         trackLength = (a1 - intersection[0]).Mag();
      } else if (IsInside(a2.X(),a2.Z())) {
         trackLength = (a2 - intersection[0]).Mag();
      }
      break;
   case 0:
      if (IsInside(a1.X(),a1.Z()) && IsInside(a2.X(),a2.Z())) {
         trackLength = (a1 - a2).Mag();
      }
      break;
   default:
      break;
   }
   return trackLength;
}

Double_t TCatReadoutPad::GetRangeToEnd(const TVector3& start, const TVector3& end) const
{
   TVector2 s(start.X(),start.Z());
   TVector2 e(end.X(),end.Z());
   TVector2 p(fX,fY);
   TVector2 v = (e-s).Unit();
   TVector2 sp = p - s;
   TVector2 x = s + (v * sp) * v;
   return (end-start).Mag() / (end.X() - start.X()) * (e.X() - x.X());

}

void TCatReadoutPad::ListOfStatus() {
   std::vector<const char*> name(3);
   std::vector<const char*> description(3);

   name[kBad] = "kBad";
   name[kGood] = "kGood";
   name[kLowGain] = "kLowGain";
   description[kBad] = "Bad channel with no signal";
   description[kGood] =    "Good channel";
   description[kLowGain] =    "Low gain channel with relatively small ammount of hits";

#if 0   
   const char* name[] = {"kBad","kGood","kLowGain"};
   const char* description[] = {
   };
#endif   
   for (Int_t i = 0; i < 3; i++) {
      printf("%-10s%-50s\n",name[i],description[i]);
   }
}

void TCatReadoutPad::SetNeighbors(const std::vector<Int_t> &neighbor)
{
   std::copy(neighbor.begin(),neighbor.end(),back_inserter(fNeighbors));
}
