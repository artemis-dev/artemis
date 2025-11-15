/**
 * @file   TPIDGateProcessor.cc
 * @brief
 *
 * @date   Created:       2014-01-28 10:18:54
 *         Last Modified: 2014-11-05 11:17:42 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TPIDGateProcessor.h"

#include <TClass.h>
#include <TH2D.h>
#include <TGraph.h>
#include <TClonesArray.h>
#include "TGateArray.h"
#include "TGateObject.h"
#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"


using art::TPIDGateProcessor;

ClassImp(art::TPIDGateProcessor);

namespace {
   const TString DEFAULT_TIMINGARRAY("def_timingarray");
   const TString DEFAULT_RECTANGLE("def_rectangle");
   const TString DEFAULT_POLYGON("def_polygon");
}

// Default constructor
TPIDGateProcessor::TPIDGateProcessor()
   : fGate(NULL), fNRectGate(0), fTMin(NULL), fTMax(NULL), fQMin(NULL), fQMax(NULL),
     fNPolyGate(0), fGraph(NULL),
     fH(NULL), fHGated(NULL) {
   RegisterProcessorParameter("TimingArray","array containing timing information",
			      fTimingColName,DEFAULT_TIMINGARRAY);
   RegisterProcessorParameter("TimingDetID","detID for timing information",
			      fTimingDetID,0);
   RegisterProcessorParameter("ChargeArray","array containing charge information",
			      fChargeColName,TString("charge"));
   RegisterProcessorParameter("ChargeDetID","detID for charge information",
			      fChargeDetID,0);

   StringVec_t defInput;
   defInput.push_back(DEFAULT_RECTANGLE);
   defInput.push_back("0");
   defInput.push_back("0");
   defInput.push_back("1");
   defInput.push_back("1");
   RegisterProcessorParameter("Rectangle","[gatename,tmin,qmin,tmax,qmin]",
			       fRectangleParList,defInput);

   StringVec_t defPolygon;
   defPolygon.push_back(DEFAULT_POLYGON);
   defPolygon.push_back("path/to/file");
   RegisterOptionalParameter("Polygon","[gatename,path/to/file]",
			     fPolygonParList,defPolygon);

   StringVec_t defOption;
   defOption.push_back("h_pid");
   defOption.push_back("h_pid;TOF;Charge");
   defOption.push_back("100");
   defOption.push_back("-10000.");
   defOption.push_back("10000.");
   defOption.push_back("100");
   defOption.push_back("-10000.");
   defOption.push_back("10000.");
   RegisterProcessorParameter("HistogramOption","name, title, xbin, xmin, xmax, ybin, ymin, ymax",
			      fHistOption, defOption);

}

TPIDGateProcessor::~TPIDGateProcessor() {
   delete fGate;
   delete fTMin;
   delete fTMax;
   delete fQMin;
   delete fQMax;
   for (Int_t i = 0 ; i != fNPolyGate; ++i) {
      delete fGraph[i];
   }
   delete fGraph;
   delete fH;
   for (Int_t i = 0; i != fNRectGate + fNPolyGate; ++i ) {
      delete fHGated[i];
   }
   delete fHGated;
}


namespace {
   inline Bool_t TestInheritance(TClonesArray **array, const TClass *base){
      const TClass *const cl = (*array)->GetClass();
      if (!cl->InheritsFrom(base) || !cl->InheritsFrom(art::TDataObject::Class())) {
	 return kFALSE;
      }
      return kTRUE;
   }

   inline TString InsertBeforeSemicolon(const TString& title, const TString& suffix) {
      return TString(title).Insert(title.First(";"),TString::Format("_%s",suffix.Data()));
   }

}

void TPIDGateProcessor::Init(TEventCollection *col)
{
   fTimingArray = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fTimingColName));
   if (!fTimingArray) return;
   if(!TestInheritance(fTimingArray,ITiming::Class())){
      Error("Init","contents of TimingArray must inherit from art::ITiming and art::TDataObject");
      return;
   }

   fChargeArray = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fChargeColName));
   if (!fChargeArray) return;
   if(!TestInheritance(fChargeArray,ICharge::Class())){
      Error("Init","contents of ChargeArray must inherit from art::ICharge and art::TDataObject");
      return;
   }

   if (fRectangleParList.front().CompareTo(DEFAULT_RECTANGLE)) {
      const Int_t rectParSize = fRectangleParList.size();
      if (rectParSize % 5) {
	 Error("Init","Invalid number of Rectangle parameter list (must be 5 * row)");
	 return;
      }
      fNRectGate = rectParSize / 5;
   }

   if (fPolygonParList.front().CompareTo(DEFAULT_POLYGON)) {
      const Int_t polyParSize = fPolygonParList.size();
      if (polyParSize % 2) {
	 Error("Init","Invalid number of Polygon parameter list (must be 2 * row)");
	 return;
      }
      fNPolyGate = polyParSize / 2;
   }

   if (!fNRectGate && !fNPolyGate) {
      Error("Init","No parameter for Rectangle or Polygon.");
      return;
   }

   if (fHistOption.size() != 8) {
      Error("Init","Invalid number of HistOption parameter list (must be 8)");
      return;
   }

   TGateArray **const gateArray = reinterpret_cast<TGateArray**>(col->GetObjectRef("gate"));
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
   fHGated = new TH2D*[fNRectGate + fNPolyGate];
   fGate = new TGateObject*[fNRectGate + fNPolyGate];

   if (fNRectGate) {
      fTMin  = new Double_t[fNRectGate];
      fTMax  = new Double_t[fNRectGate];
      fQMin  = new Double_t[fNRectGate];
      fQMax  = new Double_t[fNRectGate];

      for (Int_t i = 0; i != fNRectGate; ++i ) {
	 const TString &gateName = fRectangleParList.at(i*5);
	 fTMin[i] = fRectangleParList.at(i*5+1).Atof();
	 fQMin[i] = fRectangleParList.at(i*5+2).Atof();
	 fTMax[i] = fRectangleParList.at(i*5+3).Atof();
	 fQMax[i] = fRectangleParList.at(i*5+4).Atof();

	 fGate[i] = (*gateArray)->New(gateName,"PID Gate (Rectangle)");
	 Info("Init","new rectangle gate: %s, id: %d, t[%g:%g], q[%g:%g]",
	      gateName.Data(),(*gateArray)->GetSize(),fTMin[i],fTMax[i],fQMin[i],fQMax[i]);

	 const TString& titleGated =
	    titleHasSemiColon ? InsertBeforeSemicolon(title,gateName)
	    : title + "_" + gateName;

	 fHGated[i] = new TH2D(name+gateName,titleGated,
			       xBin,xMin,xMax,yBin,yMin,yMax);
      }
   }

   if (fNPolyGate) {
      fGraph = new TGraph*[fNPolyGate];

      for (Int_t i = 0; i != fNPolyGate; ++i ) {
	 const TString &gateName = fPolygonParList.at(i*2);
	 const TString &fileName = fPolygonParList.at(i*2 + 1);

	 fGraph[i] = new TGraph(fileName);
	 fGate[i + fNRectGate] = (*gateArray)->New(gateName,"PID Gate (Polygon)");
	 Info("Init","new polygon gate: %s, file: %s, Npoints: %d", gateName.Data(),fileName.Data(),fGraph[i]->GetN());

	 const TString& titleGated =
	    titleHasSemiColon ? InsertBeforeSemicolon(title,gateName)
	    : title + "_" + gateName;

	 fHGated[i + fNRectGate] = new TH2D(name+gateName,titleGated,
					    xBin,xMin,xMax,yBin,yMin,yMax);
      }
   }
}

namespace {
   inline const art::TDataObject *GetData(const TClonesArray *const ca,const Int_t detID) {
      const Int_t n = ca->GetEntriesFast();
      for(Int_t i = 0; i!=n;++i) {
	 const art::TDataObject *const obj =
	    static_cast<art::TDataObject*>(ca->At(i));
	 if(obj->GetID() == detID) return obj;
      }
      return NULL;
   }
}


void TPIDGateProcessor::Process() {
   if(!fGate) return;

   /* initialization is done in TGateArrayInitializer */

   /* TODO: multihit treatment */
   const ITiming *const timing = dynamic_cast<const ITiming*>(GetData(*fTimingArray,fTimingDetID));
   const ICharge *const charge = dynamic_cast<const ICharge*>(GetData(*fChargeArray,fChargeDetID));
   if (!timing || !charge) return;

   const Double_t t = timing->GetTiming();
   const Double_t q = charge->GetCharge();

   fH->Fill(t,q);

   for (Int_t i = 0; i != fNRectGate; ++i) {
      if ( t < fTMin[i] || t > fTMax[i]) continue;
      if ( q < fQMin[i] || q > fQMax[i]) continue;
      fHGated[i]->Fill(t,q);
      fGate[i]->SetState(kTRUE);
   }

   for (Int_t i = 0; i != fNPolyGate; ++i) {
      if ( fGraph[i]->IsInside(t,q) ) {
	 fHGated[i+fNRectGate]->Fill(t,q);
	 fGate[i+fNRectGate]->SetState(kTRUE);
      }
   }
}
