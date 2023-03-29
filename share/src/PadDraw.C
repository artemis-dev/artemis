#include <TMath.h>
#include <TROOT.h>

#include <TH2Poly.h>

TH2Poly *PadDraw(Int_t &nPads, Double_t *&padX, Double_t *&padY) {
   const double deg = TMath::Pi()/180.;

  Double_t l = 10. * sqrt(3)/2. * 2/3.;
  Double_t r =  9. * sqrt(3)/2. * 2/3.;
  Double_t ls = 5. * sqrt(3)/2. * 2/3.;
  Double_t rs = 4. * sqrt(3)/2. * 2/3.;
  Int_t iPads = 0;

  nPads = 400;
  padX = new Double_t[400];
  padY = new Double_t[400];
  TH2Poly *hPad = new TH2Poly("hPad","hPad",-50.,50.,-50.,50.);
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
	   padX[iPads] = (iCol+0.5)*5.-47.5;
	   x[j] += padX[iPads];
	   if ((iCol%2) == 0) {
	      if ((i+1)%2) {
		 padY[iPads] = -l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
	      } else {
		 padY[iPads] = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
	      }
	   } else {
	      if ((i)%2) {
		 padY[iPads] = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
	      } else {
		 padY[iPads] =-l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
	      }
	   }
	}
	iPads++;
	hPad->AddBin(4,x,y);
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
	   padX[iPads] = 5.*((Double_t)iCol)/2.-17.5;
	   x[j] += padX[iPads];
	   if ((iCol%2) != 0) {
	      if ((i+1)%2) {
		 padY[iPads] = -ls/2.+(ls*3.)*(i/2-4.5);
		 y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.5) ;
	      } else {
		 padY[iPads] = ls/2.+(ls*3.)*(i/2-4.5);
		 y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 4.5) ;
	      }
	   } else {
	      if ((i+1)%2) {
		 padY[iPads] = ls/2.+(ls*3.)*(i/2-5.);
		 y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 5.) ;
	      } else {
		 padY[iPads] =-ls/2.+(ls*3.)*(i/2-4.);
		 y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.) ;
	      }
	   }
	}
	iPads++;
	hPad->AddBin(4,x,y);
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
	   padX[iPads] = (iCol+0.5)*5.+17.5;
	   x[j] += padX[iPads];
	   if ((iCol%2) != 0) {
	      if ((i+1)%2) {
		 padY[iPads] = -l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
	      } else {
		 padY[iPads] = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
	      }
	   } else {
	      if ((i)%2) {
		 padY[iPads] = l/2.+(l*3.)*(i/2-2.);
		 y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
	      } else {
		 padY[iPads] =-l/2.+(l*3.)*(i/2-2.);
		 y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
	      }
	   }
	}
	iPads++;
	hPad->AddBin(4,x,y);
     }
  }
  printf("created %d pads\n",iPads);
  return hPad;
}
