/* $Id:$ */
/**
 * @file   TCatReadoutPadConditionProcessor.cc
 * @date   Created : Nov 30, 2013 22:30:49 JST
 *   Last Modified : 2017-12-21 10:30:29 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatReadoutPadConditionProcessor.h"
#include "TCatReadoutPadArray.h"
#include "TCatReadoutPad.h"
#include <TObjArray.h>
#include <TMath.h>
#include <TArtSystemOfUnit.h>
#include <TCatReadoutPad.h>

using art::TCatReadoutPadConditionProcessor;
using TArtSystemOfUnit::deg;

TCatReadoutPadConditionProcessor::TCatReadoutPadConditionProcessor()
   : fPads(NULL)
{
   fGap = 0.15;
   RegisterProcessorParameter("Type","Type 1: Center 5mm, Side 10mm",fPadType,1);
   RegisterOutputCollection("OutputName","name of pad condition",fName,TString("readoutPad"));
   RegisterProcessorParameter("Bads","list of bad channels",fBads,IntVec_t());
   RegisterProcessorParameter("LowGains","list of low gain channels",fLowGains,IntVec_t());
   RegisterProcessorParameter("Gap","full width of gap between the readout pads",fGap,fGap);
   
}
TCatReadoutPadConditionProcessor::~TCatReadoutPadConditionProcessor()
{
   if (fPads) delete fPads;
}

void TCatReadoutPadConditionProcessor::Init(TEventCollection *col)
{
   TCatReadoutPad* pad = NULL;
   fPads = new TCatReadoutPadArray;
   Construct();
   fPads->SetName(fName);
   fPads->SetBads(fBads);
   fPads->SetLowGains(fLowGains);
   for (Int_t i = 0, n = fBads.size(); i < n; ++i) {
      Int_t id = fBads[i];
      if ((pad = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(id)))) {
         pad->SetStatus(TCatReadoutPad::kBad);
      }
   }
   for (Int_t i = 0, n = fLowGains.size(); i < n; ++i) {
      Int_t id = fLowGains[i];
      if ((pad = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(id)))) {
         pad->SetStatus(TCatReadoutPad::kLowGain);
      }
   }
   col->AddInfo(fName,fPads,fOutputIsTransparent);
}


void TCatReadoutPadConditionProcessor::Construct()
{
   switch(fPadType) {
   case 1:
      Construct01();
      fPads->SetUniqueID(1);
      break;
   case 2:
      Construct02();
      fPads->SetUniqueID(2);
      break;
   case 11:
      Construct_M01();
      fPads->SetUniqueID(11);
      break;
   case 31:
      Construct_Mini();
//      Construct_M01();
      fPads->SetUniqueID(31);
      break;
   default:
      break;
   }
}


void TCatReadoutPadConditionProcessor::Construct01()
{
   Double_t gap = fGap / TMath::Sqrt(3) * 2;
  Double_t l = 10. * TMath::Sqrt(3)/2. * 2/3.;
  Double_t r =  (10-gap) * TMath::Sqrt(3)/2. * 2/3.;
  Double_t ls = 5. * TMath::Sqrt(3)/2. * 2/3.;
  Double_t rs = (5-gap) * TMath::Sqrt(3)/2. * 2/3.;
  Int_t iPads = 0;
  fNumPads = 400;
  Double_t padX, padY;

  

  
  // large pad
  for (Int_t iCol = 0; iCol < 6; iCol ++) {
     for (Int_t i=0; i<10; i++) {
	Double_t x[4] = { r*cos(-30*deg),
			  r*cos(90*deg),
			  r*cos(210*deg),
			  r*cos(-30.*deg) };
	Double_t y[4] = { r*sin(-30*deg),
			  r*sin(90*deg),
			  r*sin(210*deg),
			  r*sin(-30.*deg) };
	for (Int_t j=0;j<4;j++) {
	   padX = (iCol+0.5)*5.-47.5;
	   x[j] += padX;
	   if ((iCol%2) == 0) {
	      if ((i+1)%2) {
		 padY = -l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
	      } else {
		 padY = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
	      }
	   } else {
	      if ((i+1)%2) {
		 padY = l/2.+(l*3.)*(i/2-2.5);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
	      } else {
		 padY =-l/2.+(l*3.)*(i/2-1.5);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
	      }
	   }
	}
        Int_t neighbor[3];
        neighbor[0] = iPads-10 > -1 ? iPads-10: -1;
        neighbor[1] = iPads+10 < fNumPads? iPads+10:-1;
        neighbor[2] = -1;
//        printf("%3d %3d %3d %3d\n",iPads,neighbor[0],neighbor[1],neighbor[2]);
        fPads->Add(new TCatReadoutPad(iPads,4,padX,padY,x,y,10.,3,neighbor));
	iPads++;
     }
  }
  for (Int_t iCol = 0; iCol < 15; iCol ++) {
     for (Int_t i=0; i<20; i++) {
	Double_t x[4] = { rs*cos(-30*deg),
			  rs*cos(90*deg),
			  rs*cos(210*deg),
			  rs*cos(-30.*deg) };
	Double_t y[4] = { rs*sin(-30*deg),
			  rs*sin(90*deg),
			  rs*sin(210*deg),
			  rs*sin(-30.*deg) };
	if (iCol == 0 || iCol == 14) {
	   if ((i%4) == 0 || (i%4) == 3) continue;
	}
	for (Int_t j=0;j<4;j++) {
	   padX = 5.*((Double_t)iCol)/2.-17.5;
	   x[j] += padX;
	   if ((iCol%2) != 0) {
	      if ((i+1)%2) {
		 padY = -ls/2.+(ls*3.)*(i/2-4.5);
		 y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.5) ;
	      } else {
		 padY = ls/2.+(ls*3.)*(i/2-4.5);
		 y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 4.5) ;
	      }
	   } else {
	      if ((i+1)%2) {
		 padY = ls/2.+(ls*3.)*(i/2-5.);
		 y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 5.) ;
	      } else {
		 padY =-ls/2.+(ls*3.)*(i/2-4.);
		 y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.) ;
	      }
	   }
	}
        Int_t neighbor[3];
        if (iPads < 70 ) {
           neighbor[0] = iPads-10;
           neighbor[1] = iPads + 10 + (iPads%10)  + ((iPads%2)?0:1);
           neighbor[2] = -1;
        } else if (iPads < 90) {
           neighbor[0] = iPads + 20;
           switch ((iPads-70)%4) {
           case 0:
              neighbor[1] = (iPads - 70)/2 + 50;
              break;
           case 1:
              neighbor[1] = (iPads - 71)/2 + 60;
              break;
           case 2:
              neighbor[1] = (iPads - 70)/2 + 60;
              break;
           case 3:
              neighbor[1] = (iPads - 71)/2 + 50;
              break;
           }
           neighbor[2] = -1;
        } else if (iPads >= 330) {
           neighbor[0] = iPads+10;
           neighbor[1] = iPads -20  + (iPads%10) + ((iPads%2)?0:1);
           neighbor[2] = -1;
        } else if (iPads >= 310) {
           neighbor[0] = iPads - 20;
           switch ((iPads-310)%4) {
           case 0:
              neighbor[1] = (iPads - 310)/2 + 340;
              break;
           case 1:
              neighbor[1] = (iPads - 311)/2 + 330;
              break;
           case 2:
              neighbor[1] = (iPads - 310)/2 + 330;
              break;
           case 3:
              neighbor[1] = (iPads - 311)/2 + 340;
              break;
           }
           neighbor[2] = -1;
        } else {
           neighbor[0] = iPads-20 > -1 ? iPads-20: -1;
           neighbor[1] = iPads+20 < fNumPads? iPads+20:-1;
           neighbor[2] = -1;
        }
//        printf("%3d %3d %3d %3d\n",iPads,neighbor[0],neighbor[1],neighbor[2]);
        fPads->Add(new TCatReadoutPad(iPads,4,padX,padY,x,y,5.,3,neighbor));
	iPads++;
     }
  }
  for (Int_t iCol = 0; iCol < 6; iCol ++) {
     for (Int_t i=0; i<10; i++) {
	Double_t x[4] = { r*cos(-30*deg),
			  r*cos(90*deg),
			  r*cos(210*deg),
			  r*cos(-30.*deg) };
	Double_t y[4] = { r*sin(-30*deg),
			  r*sin(90*deg),
			  r*sin(210*deg),
			  r*sin(-30.*deg) };
	for (Int_t j=0;j<4;j++) {
	   padX = (iCol+0.5)*5.+17.5;
	   x[j] += padX;
	   if ((iCol%2) != 0) {
	      if ((i+1)%2) {
		 padY = -l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
	      } else {
		 padY = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
	      }
	   } else {
	      if ((i+1)%2) {
		 padY = l/2.+(l*3.)*(i/2-2.5);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
	      } else {
		 padY =-l/2.+(l*3.)*(i/2-1.5);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
	      }
	   }
	}
        Int_t neighbor[3];
        neighbor[0] = iPads-10;
        neighbor[1] = iPads+10 < fNumPads? iPads+10:-1;
        neighbor[2] = -1;
//        printf("%3d %3d %3d %3d\n",iPads,neighbor[0],neighbor[1],neighbor[2]);
        fPads->Add(new TCatReadoutPad(iPads,4,padX,padY,x,y,10.,3,neighbor));
	iPads++;
     }
  }
}


void TCatReadoutPadConditionProcessor::Construct02()
{
//   const Double_t r1 = 10 ; // mm
   const Double_t side = 7; // mm
   const Double_t gap = fGap;
   const Double_t r1 = (side - gap * TMath::Sqrt(3))/TMath::Sqrt(3);
   const Double_t r1nogap = side / TMath::Sqrt(3);
//   const Double_t r1 = side/TMath::Sqrt(3);
   const Int_t nCol = 32;
   const Int_t nRow = 13;
   Int_t iPads = 0;
   fNumPads = nCol * nRow;
   fPads->Expand(fNumPads);
//   Info("Construct02","Constructing readout pad type 2");
   for (Int_t iCol = 0; iCol < nCol; iCol++) {
      for (Int_t iRow = 0; iRow < nRow; iRow++) {
         Double_t r = r1;
         TVector2 vertex[4] = {
            TVector2(r*cos(-30*deg),r*sin(-30*deg)),
            TVector2(r*cos(90*deg), r*sin(90*deg)),
            TVector2(r*cos(210*deg), r*sin(210*deg)),
            TVector2(r*cos(-30.*deg), r*sin(-30.*deg))
         };
         
         Double_t x[4], y[4];
         Double_t padX = TMath::Ceil(iCol-nCol/2) * r1nogap * 3./4.;
         Double_t padY = TMath::Ceil(iRow-nRow/2) * side;
         Double_t angle = 90;
         if (iCol%2) {
            angle = 90;
            padX += side * (1./TMath::Sqrt(3.) -TMath::Sqrt(3.)/4.) ;
         } else {
            angle = -90;
            padY += side/2.;
            padX += 2 *side * (1./TMath::Sqrt(3.)-TMath::Sqrt(3.)/4.) ;
         }
         if (padX < 0) {
            switch (iCol%4) {
            case 0:
               padY -= side;
               break;
            case 1:
               break;
            case 2:
               padY -= side/2.;
               break;
            case 3:
               padY += side/2.;
               break;
            default:
               break;
            }
         }  else {
            if ((iCol/2)%2) {
               padY -= side/2.;
            }
         }
         
         angle *= 1./180.*TMath::Pi();
         for (Int_t j=0; j!=4; j++) {
            vertex[j] = vertex[j].Rotate(angle);
            x[j] = vertex[j].X()+padX;
            y[j] = vertex[j].Y()+padY;
            TVector2 vec(x[j],y[j]);
            TVector2 vecrot = vec.Rotate(TMath::Pi());
            x[j] = vecrot.X();
            y[j] = vecrot.Y();
         }
         TVector2 vec(padX,padY);
         TVector2 vecrot = vec.Rotate(TMath::Pi());
         padX = vecrot.X();
         padY = vecrot.Y();
         Int_t neighbor[3] = { 0,0,0};;
         fPads->AddAt(new TCatReadoutPad(fNumPads - iPads - 1,4,padX,padY,x,y,r1,3,neighbor),
                      fNumPads - iPads - 1 );
         iPads++;
      }
   }

   // calculate neighboring pads
   const Double_t radius = side * sqrt(3)/2.*7.*2/3.;
   for (Int_t i = 0, n = fPads->GetEntriesFast(); i < n; ++i) {
      std::vector<Int_t> neighbors;
      TCatReadoutPad *pad1 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(i));
      for (Int_t j = 0; j < n; ++j) {
         if (i == j) continue;
         TCatReadoutPad *pad2 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(j));
         Double_t distance = pad1->Distance(*pad2);
         if (distance < radius) {
            // neighbor
            neighbors.push_back(j);
         }
      }
      pad1->SetNeighbors(neighbors);
   }

}


void TCatReadoutPadConditionProcessor::Construct_M01()
{
//   const Double_t r1 = 10 ; // mm
   const Double_t side = 7; // mm
   const Double_t gap = fGap;
   const Double_t r1 = (side - gap * TMath::Sqrt(3))/TMath::Sqrt(3);
   const Double_t r1nogap = side / TMath::Sqrt(3);
//   const Double_t r1 = side/TMath::Sqrt(3);
   const Int_t nCol = 46;
   const Int_t nRow = 88;
   Int_t iPads = 0;
   Double_t padX, padY;
   fNumPads = nCol * nRow;
   const Int_t nColParted = nCol/2.;
   const Int_t nRowParted = nRow/4.;
   fPads->Expand(fNumPads);
   for (Int_t iCol = 0; iCol < nColParted; iCol++) {
      for (Int_t iRow = 0; iRow < nRowParted; iRow++) {
         Int_t id = iRow * nColParted + iCol;
         Double_t angle = 0.;
         if ((iRow%2) == (iCol%2)) {
            angle = 270;
            padX = sqrt(3)*side/2. * iCol;
            padY = iRow * side/2.;
         } else {
            angle = 90;
            padX = sqrt(3)*side/2. * iCol + side/2./sqrt(3);
            padY = iRow * side/2.;
         }

         padX += side*sqrt(3)/2. * 1./3;
         padY += side/4. - side*nRowParted;
         angle *= 1./180.*TMath::Pi();

         for (Int_t iY = 0; iY < 4; iY++) {
            Double_t r = r1;
            TVector2 vertex[4] = {
               TVector2(r*cos(-30*deg),r*sin(-30*deg)),
               TVector2(r*cos(90*deg), r*sin(90*deg)),
               TVector2(r*cos(210*deg), r*sin(210*deg)),
               TVector2(r*cos(-30.*deg), r*sin(-30.*deg))
            };
            
            Double_t x[4], y[4];
            
            for (Int_t j=0; j!=4; j++) {
               vertex[j] = vertex[j].Rotate(angle);
               x[j] = vertex[j].X()+padX;
               y[j] = vertex[j].Y()+padY + nRowParted * side/2. * iY;
            }
            Int_t neighbor[3] = { 0,0,0};;
            fPads->AddAt(new TCatReadoutPad(id+nColParted*nRowParted*iY+2024,4,padX,padY+nRowParted*side/2.*iY,x,y,r1,3,neighbor),
                         id+nColParted*nRowParted*iY+2024);
            for (Int_t j=0; j!=4; j++) {
               x[j] =  -x[j];
            }
            
            fPads->AddAt(new TCatReadoutPad(id+nColParted*nRowParted*iY,4,-padX,padY+nRowParted*side/2.*iY,x,y,r1,3,neighbor),
                         id+nColParted*nRowParted*iY);
            iPads++;
         }
      }
   }
   return;
   // calculate neighboring pads
   const Double_t radius = side * sqrt(3)/2.*7.*2/3.;
   for (Int_t i = 0, n = fPads->GetEntriesFast(); i < n; ++i) {
      std::vector<Int_t> neighbors;
      TCatReadoutPad *pad1 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(i));
      for (Int_t j = 0; j < n; ++j) {
         if (i == j) continue;
         TCatReadoutPad *pad2 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(j));
         Double_t distance = pad1->Distance(*pad2);
         if (distance < radius) {
            // neighbor
            neighbors.push_back(j);
         }
      }
      pad1->SetNeighbors(neighbors);
   }
}

void TCatReadoutPadConditionProcessor::Construct_Mini()
{
   const Double_t side = 5; 
   const Double_t gap = 0.15;
   const Double_t r1 = (side - gap * TMath::Sqrt(3))/TMath::Sqrt(3);
   const Double_t r1nogap = side / TMath::Sqrt(3);
   const Int_t nCol = 11;
   const Int_t nRow = 2;
   Int_t iPads = 0;
   Double_t padX, padY ;
   fNumPads = nCol * nRow;
   fPads->Expand(fNumPads);

  for (Int_t iCol = 0; iCol < nCol; iCol++) {
      for (Int_t iRow = 0; iRow < nRow; iRow++) {
         Int_t id = iRow * nCol + iCol;
         Double_t angle = 0.;
         if ( (id%2) ==0) {
            angle = 180;
            padY  = side*TMath::Sqrt(3)/2.*iRow - side*TMath::Sqrt(3)/6. ;
         } else {

            angle = 0;
            padY  = side*TMath::Sqrt(3)/2.*iRow - side*TMath::Sqrt(3)/3. ;
         }

         if(id<11){
             padX =  side/2. * id - 12.5;
         }else{
             padX =  side/2. * (id-11) - 12.5;
         }
         angle *= 1./180.*TMath::Pi();

         Double_t r = r1;
         TVector2 vertex[4] = {
               TVector2(r*cos(-30*deg),r*sin(-30*deg)),
               TVector2(r*cos(90*deg), r*sin(90*deg)),
               TVector2(r*cos(210*deg), r*sin(210*deg)),
               TVector2(r*cos(-30.*deg), r*sin(-30.*deg))
            };

         Double_t x[4], y[4];
         for (Int_t j=0; j!=4; j++) {
               vertex[j] = vertex[j].Rotate(angle);
               x[j] = vertex[j].X()+padX;
               y[j] = vertex[j].Y()+padY;
         }
         Int_t neighbor[3] = { 0,0,0};
         fPads->AddAt(new TCatReadoutPad(id,4,padX,padY,x,y,r1,3,neighbor),id);
         iPads++;
      }
   }
   return;
   const Double_t radius = side * sqrt(3)/2.*5.*2/3.;
   for (Int_t i = 0, n = fPads->GetEntriesFast(); i < n; ++i) {
      std::vector<Int_t> neighbors;
      TCatReadoutPad *pad1 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(i));
      for (Int_t j = 0; j < n; ++j) {
         if (i == j) continue;
         TCatReadoutPad *pad2 = dynamic_cast<TCatReadoutPad*>(fPads->UncheckedAt(j));
         Double_t distance = pad1->Distance(*pad2);
         if (distance < radius) {
            neighbors.push_back(j);
         }
      }
      pad1->SetNeighbors(neighbors);
   }
}





