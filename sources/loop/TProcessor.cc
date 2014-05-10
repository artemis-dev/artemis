/* $Id:$ */
/**
 * @file   TProcessor.cc
 * @date   Created : Jul 10, 2013 17:10:19 JST
 *   Last Modified : 2014-04-14 10:54:35 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2013
 */
#include "TProcessor.h"
#include <TClass.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

art::TProcessor::TProcessor()
   :  fInitialized(kFALSE), fParameters(NULL)
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


void art::TProcessor::Clear(Option_t *)
{
   fInitialized = kFALSE;
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
	    TParameter *prm = it->second;
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
