/**
 * @file   TPolynomialConverterArrayGenerator.cc
 * @brief
 *
 * @date   Created:       2013-10-13 16:20:19
 *         Last Modified: 2014-01-10 15:00:41
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TPolynomialConverterArrayGenerator.h"
#include "TPolynomialConverter.h"


#include <TClonesArray.h>
#include <fstream>

using art::TPolynomialConverterArrayGenerator;

ClassImp(art::TPolynomialConverterArrayGenerator)

// Default constructor
TPolynomialConverterArrayGenerator::TPolynomialConverterArrayGenerator() 
   : fConverterArrayList(NULL) {
   StringVec_t defParameterList;
   defParameterList.push_back("outputcolname");
   defParameterList.push_back("path/to/infile");
   defParameterList.push_back("1");
   defParameterList.push_back("2");

   RegisterProcessorParameter("ParameterList","format: [outputcolname,infile,nDet,degree] (number of column must be degree+1)",
			      fParameterList,defParameterList);
}

TPolynomialConverterArrayGenerator::~TPolynomialConverterArrayGenerator() {
   delete fConverterArrayList;
}

namespace {
   Bool_t LoadFile(const TString&, const Int_t, const Int_t, TClonesArray*);
}

void TPolynomialConverterArrayGenerator::Init(TEventCollection *col) {

   const Int_t nParameter = fParameterList.size();
   if (nParameter % 4) {
      Error("Init","invalid number of parameter list! (it should be 4n)\n");
      return;
   }
   const Int_t nRow = nParameter / 4;

   fConverterArrayList = new TList;
   fConverterArrayList->SetOwner(kTRUE);

   for(Int_t i = 0;i != nRow ; ++i) {
      const TString &out    = fParameterList.at(i*4+0);
      const TString &in     = fParameterList.at(i*4+1);
      const Int_t    nDet   = fParameterList.at(i*4+2).Atoi();
      const Int_t    degree = fParameterList.at(i*4+3).Atoi();

      if( !nDet || !degree ) {
	 Error("Init","invalid parameter! Please check your yaml file carefully.");
	 return;
      }

      TClonesArray *converterArray = new TClonesArray(TPolynomialConverter::Class(), 100);

      Info("Init","%s => %s",in.Data(),out.Data());
      Info("Init","nDet = %d, degree = %d", nDet, degree);
      if (!LoadFile(in, nDet, degree, converterArray)) {
	 delete converterArray;
	 continue;
      }

      converterArray->SetName(out);
      fOutputIsTransparent = kTRUE;
      col->Add(out,converterArray,fOutputIsTransparent);
      fConverterArrayList->Add(converterArray);
   }
}

namespace {
Bool_t LoadFile(const TString &file, const Int_t nDet, const Int_t degree,
		TClonesArray *converterArray) {
   std::ifstream fin(file.Data());
   if(!fin) {
      printf("LoadFile: cannot open file: %s\n",file.Data());
      return kFALSE;
   }

   const Int_t nTerm = degree + 1;
   for(Int_t iDet=0; iDet!=nDet;++iDet){
      art::TPolynomialConverter *const conv = 
	 static_cast<art::TPolynomialConverter*>(converterArray->ConstructedAt(iDet));
     
      std::vector<Double_t> tempVec(nTerm);
      for(Int_t i=0;i != nTerm;++i){
	 Double_t temp;  
	 fin >> temp;
	 tempVec[i] = temp;
      }
      conv->Set(tempVec);
      
      if(fin.eof()) {
	 printf("LoadFile: # of raw in %s is smaller than nDet!\n",file.Data());
	 return kFALSE;
      }
   }
   
   return kTRUE;
}
}
