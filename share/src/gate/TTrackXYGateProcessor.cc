/**
 * @file   TTrackXYGateProcessor.cc
 * @brief
 *
 * @date   Created:       2014-01-30 11:17:32
 *         Last Modified: 2015-04-10 15:18:46 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TTrackXYGateProcessor.h"

#include <TClass.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TMath.h>
#include <TClonesArray.h>
#include "TGateArray.h"
#include "TGateObject.h"
#include "TMWDCTrackingResult.h"

using art::TTrackXYGateProcessor;

ClassImp(art::TTrackXYGateProcessor);

namespace {
   const TString DEFAULT_INPUT("def_input");
   const TString DEFAULT_POLYGON("def_polygon");
   const TString DEFAULT_CIRCLE("def_circle");
}

// Default constructor
TTrackXYGateProcessor::TTrackXYGateProcessor()
   : fTrackArray(NULL), fGate(NULL),
     fNCircle(0), fCenterX(NULL),fCenterY(NULL), fRadius(NULL),
     fNPolygon(0), fGraph(NULL), fH(NULL), fHGated(NULL)
{
   RegisterInputCollection("Track","branch of TTrack",
			   fTrackColName,DEFAULT_INPUT,
			   &fTrackArray, "TClonesArray", "art::TTrack");

   StringVec_t defCircle;
   defCircle.push_back(DEFAULT_CIRCLE);
   defCircle.push_back("0.");
   defCircle.push_back("0.");
   defCircle.push_back("1.");
   RegisterProcessorParameter("Circle","[gatename,x,y,r]",
			      fCircleParList,defCircle);

   StringVec_t defPolygon;
   defPolygon.push_back(DEFAULT_POLYGON);
   defPolygon.push_back("path/to/file");
   RegisterProcessorParameter("Polygon","[gatename,path/to/file]",
			      fPolygonParList,defPolygon);

   StringVec_t defOption;
   defOption.push_back("h_track");
   defOption.push_back("h_track;X;Y");
   defOption.push_back("100");
   defOption.push_back("-100.");
   defOption.push_back("100.");
   defOption.push_back("100");
   defOption.push_back("-100.");
   defOption.push_back("100.");
   RegisterProcessorParameter("HistogramOption",
			      "name, title, xbin, xmin, xmax, ybin, ymin, ymax",
			      fHistOption, defOption);
}

TTrackXYGateProcessor::~TTrackXYGateProcessor()
{
   delete fGate;
   delete fCenterX;
   delete fCenterY;
   delete fRadius;
   for (Int_t i = 0; i != fNPolygon; ++i ) {
      delete fGraph[i];
      delete fHGated[i];
   }
   delete fGraph;
   delete fH;
   delete fHGated;
}

namespace {
   inline TString InsertBeforeSemicolon(const TString& title, const TString& suffix) {
      return TString(title).Insert(title.First(";"),TString::Format("_%s",suffix.Data()));
   }
}

void TTrackXYGateProcessor::Init(TEventCollection *col)
{
   if (fCircleParList.front().CompareTo(DEFAULT_CIRCLE)) {
      const Int_t circParSize = fCircleParList.size();
      if (circParSize % 4) {
	 Error("Init","Invalid number of Circle parameter list (must be 4 * row)");
	 return;
      }
      fNCircle = circParSize / 4;
   }

   if (fPolygonParList.front().CompareTo(DEFAULT_POLYGON)) {
      const Int_t polyParSize = fPolygonParList.size();
      if (polyParSize % 2) {
	 Error("Init","Invalid number of Polygon parameter list (must be 2 * row)");
	 return;
      }
      fNPolygon = polyParSize / 2;
   }

   if (!fNPolygon && !fNCircle) {
	 Error("Init","Parameter option 'Polygon' or 'Circle' is necessary.");
	 return;
   }

   if (fHistOption.size() != 8) {
      Error("Init","Invalid number of HistOption parameter list (must be 8)");
      return;
   }

   if (!CheckHistOption()){
      return;
   }

   TGateArray **const gateArray =
      reinterpret_cast<TGateArray**>(col->GetObjectRef("gate"));
   if(!gateArray) {
      Error("Init","gate array not found. Run TGateArrayInitializer before this.");
      return;
   }

   const TString  name = fHistOption.at(0);
   const TString title = fHistOption.at(1);
   const Int_t    xBin = fHistOption.at(2).Atoi();
   const Double_t xMin = fHistOption.at(3).Atof();
   const Double_t xMax = fHistOption.at(4).Atof();
   const Int_t    yBin = fHistOption.at(5).Atoi();
   const Double_t yMin = fHistOption.at(6).Atof();
   const Double_t yMax = fHistOption.at(7).Atof();

   const Bool_t titleHasSemiColon = title.Contains(";");

   fH = new TH2D(name,title,xBin,xMin,xMax,yBin,yMin,yMax);
   const Int_t nGate = fNCircle + fNPolygon;
   fHGated = new TH2D*[nGate];
   fGate = new TGateObject*[nGate];

   if (fNCircle) {
      fCenterX = new Double_t[fNCircle];
      fCenterY = new Double_t[fNCircle];
      fRadius  = new Double_t[fNCircle];

      for (Int_t i = 0; i!= fNCircle ; ++i ) {
	 const TString &gateName = fCircleParList.at(i*4);
	 fCenterX[i] = fCircleParList.at(i*4+1).Atof();
	 fCenterY[i] = fCircleParList.at(i*4+2).Atof();
	 fRadius[i]  = fCircleParList.at(i*4+3).Atof();

	 fGate[i] = (*gateArray)->New(gateName,"TrackXYGate (Circle)");
	 Info("Init","new circle gate: %s, center: (%lf, %lf), Radius: %lf",
	      gateName.Data(),fCenterX[i], fCenterY[i], fRadius[i]);

	 const TString& titleGated =
	    titleHasSemiColon ? InsertBeforeSemicolon(title,gateName)
	    : title + "_" + gateName;

	 fHGated[i] = new TH2D(name+gateName,titleGated,
					xBin,xMin,xMax,yBin,yMin,yMax);
      }
   }

   if (fNPolygon) {
      fGraph = new TGraph*[fNPolygon];

      for (Int_t i = 0; i != nGate; ++i ) {
	 const TString &gateName = fPolygonParList.at(i*2);
	 const TString &fileName = fPolygonParList.at(i*2 + 1);

	 fGraph[i] = new TGraph(fileName);
	 fGate[i + fNCircle] = (*gateArray)->New(gateName,"TrackXYGate (Polygon)");
	 Info("Init","new polygon gate: %s, file: %s, Npoints: %d",
	      gateName.Data(),fileName.Data(),fGraph[i]->GetN());

	 const TString& titleGated =
	    titleHasSemiColon ? InsertBeforeSemicolon(title,gateName)
	    : title + "_" + gateName;

	 fHGated[i+fNCircle] = new TH2D(name+gateName,titleGated,
			       xBin,xMin,xMax,yBin,yMin,yMax);
      }
   }
}

void TTrackXYGateProcessor::Process() {

   if(!fGate) return;

   /* initialization is done in TGateArrayInitializer */

   const TTrack *track = static_cast<const TTrack*>((*fTrackArray)->At(0));
   if (!track) return;

   const Double_t x = track->X();
   const Double_t y = track->Y();

   fH->Fill(x,y);

   for (Int_t i = 0; i != fNCircle; ++i) {
      const Double_t dx = x - fCenterX[i];
      const Double_t dy = y - fCenterY[i];
      const Double_t r2  = TMath::Power(fRadius[i],2);
      if (dx*dx + dy*dy < r2) {
	 fHGated[i]->Fill(x,y);
	 *fGate[i] = kTRUE;
      }
   }

   for (Int_t i = 0; i != fNPolygon; ++i) {
      if ( fGraph[i]->IsInside(x,y) ) {
	 fHGated[i+fNCircle]->Fill(x,y);
	 *fGate[i+fNCircle] = kTRUE;
      }
   }
}

Bool_t TTrackXYGateProcessor::CheckHistOption() const {
   if (!fHistOption.at(2).IsDigit()) {
      Error("CheckHistOption","3rd value of HistOption must be Integer (value: %s)",
	    fHistOption.at(2).Data());
      return kFALSE;
   }

   if (!fHistOption.at(3).IsFloat()) {
      Error("CheckHistOption","4th value of HistOption must be Float (value: %s)",
	    fHistOption.at(3).Data());
      return kFALSE;
   }

   if (!fHistOption.at(4).IsFloat()) {
      Error("CheckHistOption","5th value of HistOption must be Float (value: %s)",
	    fHistOption.at(4).Data());
      return kFALSE;
   }

   if (!fHistOption.at(5).IsDigit()) {
      Error("CheckHistOption","6th value of HistOption must be Integer (value: %s)",
	    fHistOption.at(5).Data());
      return kFALSE;
   }

   if (!fHistOption.at(6).IsFloat()) {
      Error("CheckHistOption","7th value of HistOption must be Float (value: %s)",
	    fHistOption.at(6).Data());
      return kFALSE;
   }

   if (!fHistOption.at(7).IsFloat()) {
      Error("CheckHistOption","8th value of HistOption must be Float (value: %s)",
	    fHistOption.at(7).Data());
      return kFALSE;
   }

   return kTRUE;
}
