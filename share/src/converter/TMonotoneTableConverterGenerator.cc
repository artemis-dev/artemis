/**
 * @file   TMonotoneTableConverterGenerator.cc
 * @brief
 *
 * @date   Created:       2013-10-18 10:53:58
 *         Last Modified: 2014-01-10 14:52:37
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMonotoneTableConverterGenerator.h"
#include "TMonotoneTableConverter.h"

#include <TClonesArray.h>
#include <fstream>
#include <string>

using art::TMonotoneTableConverterGenerator;

namespace {
inline Bool_t LoadFile(const TString&, art::TMonotoneTableConverter*);
}

ClassImp(art::TMonotoneTableConverterGenerator)

// Default constructor
TMonotoneTableConverterGenerator::TMonotoneTableConverterGenerator() 
   : fConverterList(NULL) {
   StringVec_t defParameterList;
   defParameterList.push_back("monotone_table_conversion");
   defParameterList.push_back("path/to/infile");

   RegisterProcessorParameter("ParameterList","format: [outputcolname,infile]",
			      fParameterList,defParameterList);
}

TMonotoneTableConverterGenerator::~TMonotoneTableConverterGenerator() {
   delete fConverterList;
}

void TMonotoneTableConverterGenerator::Init(TEventCollection *col) {

   const Int_t nParameter = fParameterList.size();
   if (nParameter % 2) {
      Error("Init","Invalid number of parameter list! (it should be 2n)");
      return;
   }
   const Int_t nRow = nParameter / 2;
 
   fConverterList = new TList;
   fConverterList->SetOwner(kTRUE);

   for(Int_t i = 0;i != nRow ; ++i) {
      const TString &out = fParameterList.at(i*2);
      const TString &in  = fParameterList.at(i*2+1);
      
      TMonotoneTableConverter *converter = new TMonotoneTableConverter;
      
      Info("Init","%s => %s",in.Data(),out.Data());
      if (!LoadFile(in,converter)) { 
	 delete converter;
	 continue;
      }

      fOutputIsTransparent = kTRUE;
      col->Add(out,converter,fOutputIsTransparent);
      fConverterList->Add(converter);
   }
}

namespace {

Bool_t LoadFile(const TString &file, art::TMonotoneTableConverter *converter) {

   std::ifstream ifs(file.Data());
   if (ifs.fail()){
      printf("LoadFile: Cannot open file:%s\n",file.Data());
      return kFALSE;
   }

   Int_t nPoints = 0;
   ifs >> nPoints;

   if (nPoints < 1) return kFALSE;

   Double_t *const x = new Double_t [nPoints];
   Double_t *const y = new Double_t [nPoints];

// TODO: monotonic check
   {
      Int_t i = 0;
      std::string str;
      while (ifs && std::getline(ifs,str,'\n')) {
	 if (2 != std::sscanf(str.c_str(), "%lf %lf %*s", &(x[i]), &(y[i]))) {
	    continue;
	 }
	 ++i;
	 if (i == nPoints) break;
      }
      if (i != nPoints) nPoints = i;
   }

   converter->Set(x,y,nPoints);

   delete [] x;
   delete [] y;

   return kTRUE;
}

} // end of anonymous namespace

