/**
 * @file   TAffineConverterConditionProcessor.cc
 * @brief  time dependent affine converter
 *
 * @date   Created       : 2016-11-15 18:02:32 JST
 *         Last Modified : 2019-09-09 19:07:04 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TAffineConverterConditionProcessor.h"
#include <TEventHeader.h>
#include <TAffineConverter.h>
#include <TClonesArray.h>
#include <fstream>

using art::TAffineConverterConditionProcessor;

ClassImp(TAffineConverterConditionProcessor)

TAffineConverterConditionProcessor::TAffineConverterConditionProcessor()
: fConverterArrayList(NULL) {
   StringVec_t defParameterList;
   defParameterList.push_back("affine_conversion");
   defParameterList.push_back("path/to/infile");
   
   RegisterProcessorParameter("ParameterList","format: [outputcolname,infile]",
			      fParameterList,defParameterList);
   RegisterInputCollection("NameEventHeader","name of event header",
                              fNameEventHeader,TString("eventheader"),
                              &fEventHeader,TEventHeader::Class_Name());
   Register(fTypeOfTimestamp("TypeOfTimestamp","type of timestamp, 0 : eventnumber, 1 : runnumber, 2: timestamp",0));
}

TAffineConverterConditionProcessor::~TAffineConverterConditionProcessor()
{
   if (fConverterArrayList) {
      delete fConverterArrayList;
   }
}

TAffineConverterConditionProcessor::TAffineConverterConditionProcessor(const TAffineConverterConditionProcessor& rhs)
{
}

TAffineConverterConditionProcessor& TAffineConverterConditionProcessor::operator=(const TAffineConverterConditionProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TAffineConverterConditionProcessor::Init(TEventCollection *col) {
   const Int_t nParameter = fParameterList.size();
   if (nParameter % 2) {
      SetStateError("Invalid number of parameter list! (it should be 2n)");
      return;
   }
   const Int_t nRow = nParameter / 2;
   if (nRow > 1) {
      SetStateError("only one file can be treated");
      return;
   }

   fConverterArrayList = new TList;
   fConverterArrayList->SetOwner(kTRUE);
   for(Int_t i = 0;i != nRow ; ++i) {
      const TString &out = fParameterList.at(i*2);
      const TString &in  = fParameterList.at(i*2+1);
      TClonesArray *converterArray = new TClonesArray(TAffineConverter::Class(),100);

      Info("Init","%s => %s",in.Data(),out.Data());
      if (!LoadFile(in,converterArray)) {
         SetStateError(Form("Can't load file '%s'",in.Data()));
	 delete converterArray;
         return;
      }
      converterArray->SetName(out);
      fOutputIsTransparent = kTRUE;
      col->AddInfo(out,converterArray,fOutputIsTransparent);
      fConverterArrayList->Add(converterArray);
   }

   if (fTypeOfTimestamp != 0 &&
       fTypeOfTimestamp != 1) {
      SetStateError(Form("Unknown timestamp type %d",(Int_t)fTypeOfTimestamp));
   }
}

void TAffineConverterConditionProcessor::Process()
{
   if (fNextTimestamp < 0) return;
   double timestamp = 0;
   switch (fTypeOfTimestamp) {
   case 0: {
      timestamp = (*fEventHeader)->GetEventNumber() ;
      break;
   }
   case 1: {
      timestamp = (*fEventHeader)->GetRunNumber();
      break;
   }
   default:
      break;
   }
   if (timestamp <= fNextTimestamp) return;
   if (fNextIdx >= fTimestamp[0].size()) return;
//   printf("update fNextTimestamp = %f\n",fNextTimestamp);
   
   // condition is filled
   TClonesArray *converterArray = dynamic_cast<TClonesArray*>(fConverterArrayList->At(0));
   if (!converterArray) {
      SetStopLoop();
      SetStateError("no converter array is available");
      return;
   }
   while (timestamp >= fNextTimestamp) {
      Int_t n = converterArray->GetEntriesFast();
      for (Int_t i = 0; i < n; ++i) {
         TAffineConverter *conv = static_cast<TAffineConverter*>(converterArray->UncheckedAt(i));
         conv->Set(fA0[i][fNextIdx],fA1[i][fNextIdx]);
//         printf("i/n = %d/%d,%f %f\n",i,n,fA0[i][fNextIdx],fA1[i][fNextIdx]);
      }
      ++fNextIdx;
      if (fTimestamp[0].size() ==  fNextIdx) break;
      fNextTimestamp = fTimestamp[0][fNextIdx];
   }
   
}


Bool_t TAffineConverterConditionProcessor::LoadFile(const TString& file, TClonesArray* converterArray) {
   std::ifstream ifs(file.Data());
   if(ifs.fail()) {
      printf("LoadFile: Cannot open file: %s\n",file.Data());
      return kFALSE;
   }
   for (Int_t i = 0;;) {
      std::string str;
      if (!ifs || !std::getline(ifs,str,'\n')) break;
         
      Int_t id;
      Double_t timestamp, a0,a1;
      if (4 != std::sscanf(str.c_str(), "%d %lf %lf %lf %*s",&id, &timestamp, &a0, &a1)) {
         continue;
      }
//      if (id < 10)
//      printf("%d %lf %lf %lf\n",id,timestamp,a0,a1);
      if (fTimestamp.size() < id + 1) {
         fTimestamp.resize(id + 1);
         fA0.resize(id + 1);
         fA1.resize(id + 1);
      }
      fTimestamp[id].push_back(timestamp);
      fA0[id].push_back(a0);
      fA1[id].push_back(a1);
      ++i;
   }
   if (fTimestamp.size() == 0) {
      Warning("LoadFile","Parameter is not set");
      return kFALSE;
   }
   for (Int_t i = 0, n = fTimestamp.size(); i < n; ++i) {
      if (fTimestamp[i].size() == 0) {
         Warning("LoadFile","Parameter for id = %d is not set\n",i);
         return kFALSE;
      }
      art::TAffineConverter *const conv = 
         static_cast<art::TAffineConverter*>(converterArray->ConstructedAt(i));
      conv->Set(fA0[i][0],fA1[i][0]);
   }
   if (fTimestamp[0].size() > 1) {
      fNextTimestamp = fTimestamp[0][1];
      fNextIdx = 1;
   } else {
      fNextTimestamp = -1;
      fNextIdx = -1;
   }
   return kTRUE;
}

