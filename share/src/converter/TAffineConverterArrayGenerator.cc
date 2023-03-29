/**
 * @file   TAffineConverterArrayGenerator.cc
 * @brief  generates array of TAffineConverter
 *
 * @date   Created:       2013-10-14 15:22:07
 *         Last Modified: 2014-01-10 14:58:11
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TAffineConverterArrayGenerator.h"
#include "TAffineConverter.h"

#include <TClonesArray.h>
#include <fstream>
#include <string>
#include <cstdio>

using art::TAffineConverterArrayGenerator;

ClassImp(art::TAffineConverterArrayGenerator)

// Default constructor
TAffineConverterArrayGenerator::TAffineConverterArrayGenerator() 
: fConverterArrayList(NULL) {
   StringVec_t defParameterList;
   defParameterList.push_back("affine_conversion");
   defParameterList.push_back("path/to/infile");
   
   RegisterProcessorParameter("ParameterList","format: [outputcolname,infile]",
			      fParameterList,defParameterList);
}

TAffineConverterArrayGenerator::~TAffineConverterArrayGenerator() {
   delete fConverterArrayList;
}

namespace {
   Bool_t LoadFile(const TString&, TClonesArray*);
}

void TAffineConverterArrayGenerator::Init(TEventCollection *col) {

   const Int_t nParameter = fParameterList.size();
   if (nParameter % 2) {
      Error("Init","Invalid number of parameter list! (it should be 2n)");
      return;
   }
   const Int_t nRow = nParameter / 2;

   fConverterArrayList = new TList;
   fConverterArrayList->SetOwner(kTRUE);

   for(Int_t i = 0;i != nRow ; ++i) {
      const TString &out = fParameterList.at(i*2);
      const TString &in  = fParameterList.at(i*2+1);

      TClonesArray *converterArray = new TClonesArray(TAffineConverter::Class(),100);

      Info("Init","%s => %s",in.Data(),out.Data());
      if (!LoadFile(in,converterArray)) {
	 delete converterArray;
	 continue;
      }

      converterArray->SetName(out);
      fOutputIsTransparent = kTRUE;
      col->AddInfo(out,converterArray,fOutputIsTransparent);
      fConverterArrayList->Add(converterArray);
   }
}

namespace {
Bool_t LoadFile(const TString &file, TClonesArray* converterArray) {

   std::ifstream ifs(file.Data());

   if(ifs.fail()) {
      printf("LoadFile: Cannot open file: %s\n",file.Data());
      return kFALSE;
   }

   for (Int_t i = 0;;) {
      std::string str;
      if (!ifs || !std::getline(ifs,str,'\n')) break;

      Double_t a0,a1;
      if (2 != std::sscanf(str.c_str(), "%lf %lf %*s", &a0, &a1)) {
	 continue;
      }

      art::TAffineConverter *const conv = 
	 static_cast<art::TAffineConverter*>(converterArray->ConstructedAt(i++));
      conv->Set(a0,a1);
   }
   return kTRUE;
}
}
