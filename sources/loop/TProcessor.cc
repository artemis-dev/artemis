/* $Id:$ */
/**
 * @file   TProcessor.cc
 * @date   Created : Jul 10, 2013 17:10:19 JST
 *   Last Modified : Sep 17, 2013 18:13:29 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TProcessor.h"
#include <TClass.h>
#include <TROOT.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

art::TProcessor::TProcessor()
   :  fInitialized(kFALSE), fParameters(NULL)
{
   RegisterOptionalParameter("OutputTransparency","Output is persistent if false (default)",
                             fOutputIsTransparent,kFALSE);
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
   YAML::Emitter out;
   out << YAML::BeginMap;
   {
      out << YAML::Key << "Processor";
      out << YAML::Value;
      out << YAML::BeginMap;
      {
         out << YAML::Key << "name" << YAML::Value 
             << TString::Format("My%s",GetTitle())
             << YAML::Key << "type" << YAML::Value 
             << TString::Format("%s",GetTitle())
             << YAML::Key << "parameter" << YAML::Value;

         out << YAML::BeginSeq;
         ProcPrmMap_t::iterator it;
         for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
            TParameter *prm = it->second;
            out << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << prm->GetName();
            out << YAML::Comment(prm->GetTitle().Data());
            out << YAML::Key << "type" << YAML::Value << prm->Type()
                << YAML::Key << "size" << YAML::Value << prm->Size()
                << YAML::Key << "value" << YAML::Value 
                << YAML::BeginSeq
                << prm->DefaultValue()
                << YAML::EndSeq;
            out << YAML::EndMap;
         }
         out << YAML::EndSeq;

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
   TClass *cls = gROOT->GetClass(type.data());
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
