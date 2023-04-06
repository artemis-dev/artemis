/**
 * @file   TPolynomialConverterGenerator.cc
 * @brief
 *
 * @date   Created:       2013-12-04 11:33:04
 *         Last Modified: 2014-01-10 15:03:34
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TPolynomialConverterGenerator.h"
#include "TPolynomialConverter.h"

#include <fstream>

#include <TList.h>

using art::TPolynomialConverterGenerator;

ClassImp(TPolynomialConverterGenerator)

// Default constructor
TPolynomialConverterGenerator::TPolynomialConverterGenerator() 
   : fConverterList(NULL) {
   StringVec_t defParameterList;
   defParameterList.push_back("outputname");
   defParameterList.push_back("path/to/infile");
   defParameterList.push_back("1");
   defParameterList.push_back("2");

   RegisterProcessorParameter("ParameterList","format: [outputname,infile,degree] (number of column must be degree+1)",
			      fParameterList,defParameterList);
}

TPolynomialConverterGenerator::~TPolynomialConverterGenerator() {
   delete fConverterList;
}

namespace {
   Bool_t LoadFile(const TString&, Int_t, art::TPolynomialConverter*);
}

void TPolynomialConverterGenerator::Init(TEventCollection *col) {

   const Int_t nParameter = fParameterList.size();
   if (nParameter % 3) {
      Error("Init","Invalid number of parameter list! (it should be 3n)");
      return;
   }
   const Int_t nRow = nParameter / 3;
 
   fConverterList = new TList;
   fConverterList->SetOwner(kTRUE);
  
   for(Int_t i = 0;i != nRow ; ++i) {
      const TString &out  = fParameterList.at(i*3);
      const TString &in   = fParameterList.at(i*3+1);
      const Int_t   &deg  = fParameterList.at(i*3+2).Atoi();

      TPolynomialConverter *converter = new TPolynomialConverter;
      
      Info("Init","%s => %s (degree: %d)",in.Data(),out.Data(), deg);
      if (!LoadFile(in,deg,converter)) {
	 delete converter;
	 continue;
      }

      fOutputIsTransparent = kTRUE;
      col->Add(out,converter,fOutputIsTransparent);
      fConverterList->Add(converter);
   }
}

namespace {
Bool_t LoadFile(const TString &file, const Int_t degree,
		art::TPolynomialConverter *converter) {
   std::ifstream fin(file.Data());

   if(!fin) {
      printf("LoadFile: cannot open file: %s\n",file.Data());
      return kFALSE;
   }

   const Int_t nTerm = degree + 1;
   std::vector<Double_t> tempVec(nTerm);
   for(Int_t i=0;i != nTerm;++i){
      Double_t temp;
      fin >> temp;
      tempVec[i] = temp;
   }
   converter->Set(tempVec);

   return kTRUE;
}
}
