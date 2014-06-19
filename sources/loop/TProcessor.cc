/* $Id:$ */
/**
 * @file   TProcessor.cc
 * @date   Created : Jul 10, 2013 17:10:19 JST
 *   Last Modified : Jun 19, 2014 16:53:29 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2013
 */
#include "TProcessor.h"
#include <TClass.h>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <TClonesArray.h>

art::TProcessor::TProcessor()
   :   fParameters(NULL), fState(INIT), fErrorMessage("")
{
   RegisterOptionalParameter("OutputTransparency",
			     "Output is persistent if false (default)",
                             fOutputIsTransparent,kFALSE);
   RegisterOptionalParameter("Verbose",
			     "verbose level (default 1 : non quiet)",
			     fVerboseLevel,1);
}

art::TProcessor::~TProcessor()
{
   // delete parameter strings if any
   if (fParameters) delete fParameters;
   fParameters = NULL;

   // delete parameters
   ProcPrmMap_t::iterator it;
   for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
      delete it->second;
   }
}

void art::TProcessor::InitProc(TEventCollection *col)
{
   Info("InitProc","Initilizing ...");
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
   // obtain input collection
   Int_t nInputs = fInputs.size();
   Int_t iInput = 0;
   for (iInput = 0; iInput != nInputs; iInput++)  {
      IOCollection &input = fInputs[iInput];
      *(void***)input.fP = NULL;
      TString inputname = *input.fName;
      if (!(col->GetObjectRef(inputname))) {
         SetStateError(TString::Format(ErrMsgFmt::INVALID_INPUT_COLLECTION,inputname.Data()));
         return;
      }
      // initialize input collection
      *(void***)input.fP = (void**) col->GetObjectRef(inputname);
      TObject *obj = **((TObject***)(void***)input.fP);

      // check if the input class match
      if (!obj->IsA()->InheritsFrom(input.fClassName)) {
         SetStateError(TString::Format(ErrMsgFmt::INPUT_CLASS_MISSMATCH,input.fClassName.Data(),obj->IsA()->GetName()));
         return;
      }

      // check if the input data class for TClonesArray matches
      if (TClass::GetClass(input.fClassName)->InheritsFrom("TClonesArray")) {
         TClonesArray *arr = static_cast<TClonesArray*>(obj);
         // check the pointer null
         if (!arr->GetClass()) {
            SetStateError(TString::Format(ErrMsgFmt::INPUT_DATA_CLASS_MISSMATCH,input.fDataClassName.Data(),"nil"));
            return;
         }
         // check the class inherits from the required data
         if (!arr->GetClass()->InheritsFrom(input.fDataClassName)) {
            SetStateError(TString::Format(ErrMsgFmt::INPUT_DATA_CLASS_MISSMATCH,input.fDataClassName.Data(),arr->GetClass()->GetName()));
            return;
         }
      }
   }

   Int_t nOutputs = fOutputs.size();
   Int_t iOutput = 0;
   for (iOutput = 0; iOutput != nOutputs; iOutput++) {
      IOCollection &output = fOutputs[iOutput];
      TClass *cls = TClass::GetClass(output.fClassName);

      // check output class exists
      if (!cls) {
         SetStateError(TString::Format(ErrMsgFmt::NOT_EXIST_CLASS,output.fClassName.Data()));
         return;
      }
      // check output data class exists
      if (cls == TClonesArray::Class() && 
          !TClass::GetClass(output.fDataClassName)) {
         SetStateError(TString::Format(ErrMsgFmt::NOT_EXIST_DATA_CLASS,output.fDataClassName.Data()));
         return;
      }
      // check output collection exist
      if (col->GetObjectRef(*output.fName)) {
         SetStateError(TString::Format(ErrMsgFmt::OUTPUT_ALREADY_EXIST,output.fName->Data()));
         return;
      }
         
      // prepare output object
      *(void**)output.fP = (void*) cls->New();
      if (cls == TClonesArray::Class()) {
         TClonesArray *arr = static_cast<TClonesArray*>(*(void**)output.fP);
         // printf("setting %s%p\n",output.fDataClassName.Data(),TClass::GetClass(output.fDataClassName));
         arr->SetClass(TClass::GetClass(output.fDataClassName));
         // printf("set\n");
      }
      col->Add(*output.fName,(TObject*)*(void**)output.fP,fOutputIsTransparent);
   }

   Int_t nInputInfo = fInputInfo.size();
   for (Int_t iInfo = 0; iInfo != nInputInfo; iInfo++) {
      IOCollection &info = fInputInfo[iInfo];
      TClass *cls = TClass::GetClass(info.fClassName);
      TString infoname = *info.fName;
      if (!fParamMap[info.fPrmName]) {
         SetStateError(TString::Format(ErrMsgFmt::INVALID_INPUT_COLLECTION,info.fPrmName.Data()));
         return;
      }
      // get registered information
      if (!((*(void***)info.fP) = (void**)col->GetInfoRef(infoname)) && !fParamMap[info.fPrmName]->IsOptional()) {
         SetStateError(TString::Format(ErrMsgFmt::INVALID_INPUT_COLLECTION,infoname.Data()));
         return;
      }
      // object should be casted 
      TObject *obj = **(TObject***)((void***)info.fP);

      // check if the info class match
      if (!obj->IsA()->InheritsFrom(info.fClassName)) {
         SetStateError(TString::Format(ErrMsgFmt::INPUT_CLASS_MISSMATCH,info.fClassName.Data(),obj->IsA()->GetName()));
         return;
      }

      // check if the info data class for TClonesArray matches
      if (TClass::GetClass(info.fClassName)->InheritsFrom("TClonesArray")) {
         TClonesArray *arr = static_cast<TClonesArray*>(obj);
         // check the pointer null
         if (!arr->GetClass()) {
            SetStateError(TString::Format(ErrMsgFmt::INPUT_DATA_CLASS_MISSMATCH,info.fDataClassName.Data(),"nil"));
            return;
         }
         // check the class inherits from the required data
         if (!arr->GetClass()->InheritsFrom(info.fDataClassName)) {
            SetStateError(TString::Format(ErrMsgFmt::INPUT_DATA_CLASS_MISSMATCH,info.fDataClassName.Data(),arr->GetClass()->GetName()));
            return;
         }
      }
   }         
      

   // call user defined initialization function
   Init(col);
   if (IsError()) {
      return;
   } else {
      // initializaed correctly
      SetStateReady();
   }
   Info("InitProc","Done");
}   


void art::TProcessor::Clear(Option_t *)
{

}




void art::TProcessor::SetName(const char *name)
{
   TNamed::SetTitle(IsA()->GetName());
   TNamed::SetName(name);
//   printf("ClassName() %s\n",IsA()->GetName());
}

void art::TProcessor::SetParameters(TParameterStrings *params)
{
   if (fParameters) delete fParameters;
   // own the parameters
   fParameters = params;
   // update parameters using new parameter string
   UpdateParameters();
}

void art::TProcessor::UpdateParameters()
{
   ProcPrmMap_t::iterator it;
   for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
      it->second->SetValue(fParameters);
   }
}

void art::TProcessor::PrintDescriptionYAML()
{
   TString name;
   if (strcmp(GetName(),"")){
      name = GetName();
   } else {
      TString classname = IsA()->GetName();
      name = TString::Format("My%s",classname.Remove(0,classname.Last(':')+1).Data());
   }

   const TString& title =
	 strcmp(GetTitle(),"") ? TString(GetTitle()) : TString(IsA()->GetName());

   YAML::Emitter out;
   out << YAML::BeginMap;
   {
      out << YAML::Key << "Processor";
      out << YAML::Value;
      out << YAML::BeginSeq;
      out << YAML::BeginMap;
      {
         out << YAML::Key << "name" << YAML::Value
             << name.Data()
             << YAML::Key << "type" << YAML::Value
             << title.Data()
             << YAML::Key << "parameter" << YAML::Value;
         out << YAML::BeginMap;
         ProcPrmMap_t::iterator it;
         for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
	    TProcessorParameter *prm = it->second;
	    const TString &value =
	       prm->IsValueSet() ? prm->Value() : prm->DefaultValue();
	    const TString &comment =
	       TString::Format("[%s] %s",
			       prm->Type().Data(), prm->GetTitle().Data());
	    out << YAML::Key << prm->GetName();
	    if (prm->IsStringVector()) {
	       out << YAML::Comment(comment.Data())
		   << YAML::Value
		   << YAML::BeginSeq
		   << value.Data()
		   << YAML::EndSeq;
	    } else if (prm->IsVector()) {
	       TObjArray *values = value.Tokenize(", ");
	       const Int_t n = values->GetEntriesFast();
	       out << YAML::Value
		   << YAML::Flow
		   << YAML::BeginSeq;
	       for(int i = 0; i != n ; ++i) {
		  out << ((TObjString*)values->At(i))->GetString().Data();
	       }
	       out << YAML::EndSeq;
	       out << YAML::Comment(comment.Data());
	    } else {
	       out << YAML::Value
		   << value.Data()
		   << YAML::Comment(comment.Data());
	    }
         }
	 out << YAML::EndSeq;
         out << YAML::EndMap;
      }
      out   << YAML::EndMap;
   }
   out << YAML::EndMap;
   std::cout << out.c_str() << std::endl;

}

void operator >> (const YAML::Node &node, art::TProcessor *&proc)
{
   std::string name, type;
   proc = NULL;
   try {
      node["name"] >> name;
      node["type"] >> type;
   } catch (YAML::KeyNotFound& e) {
      std::cout << e.what() << std::endl;
      return;
   }
   TClass *cls = TClass::GetClass(type.data());
   if (!cls) {
      std::cout << "  no such processor, or processor is not register in dictionary" << std::endl;
      std::cout << "  " << name << " " << type  << std::endl;
      return;
   }
   proc = (art::TProcessor*) cls->New();
   art::TParameterStrings *str = new art::TParameterStrings;
   try {
      node["parameter"] >> str;
   } catch (YAML::KeyNotFound& e) {
      // nothing to do with no paramter for now
      // std::cout << e.what() << std::endl;
   }
   proc->SetParameters(str);
   proc->SetName(name.data());
}
