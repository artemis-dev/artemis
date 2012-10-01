/* $Id:$ */
/**
 * @file   TCatProcessor.cc
 * @date   Created : Feb 19, 2012 12:19:11 JST
 *   Last Modified : May 19, 2012 17:13:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatProcessor.h"

#include <TROOT.h>
#include <TFolder.h>

#include <yaml-cpp/yaml.h>



TCatProcessor::TCatProcessor()
   :  fParameters(NULL), fWidget(NULL), fInitialized(kFALSE)
{
   RegisterOptionalParameter("OutputTransparency","Output is persistent if false (default)",
                             fOutputIsTransparent,kFALSE);
}

TCatProcessor::~TCatProcessor()
{
   // delete parameter strings if any
   if (fParameters) delete fParameters;
   fParameters = NULL;

   // delete parameters
   CatProcPrmMap_t::iterator it;
   for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
      delete it->second;
   }
}


void TCatProcessor::Clear(Option_t *)
{
   fInitialized = kFALSE;
}

void TCatProcessor::Initilized()
{
   fInitialized = kTRUE;
}


void TCatProcessor::SetName(const char *name)
{
   TNamed::SetTitle(IsA()->GetName());
   TNamed::SetName(name);
//   printf("ClassName() %s\n",IsA()->GetName());
}

void TCatProcessor::SetParameters(TCatParameterStrings *params)
{
   if (fParameters) delete fParameters;
   // own the parameters
   fParameters = params;
   // update parameters using new parameter string 
   UpdateParameters();
}

void TCatProcessor::UpdateParameters()
{
   CatProcPrmMap_t::iterator it;
   for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
      it->second->SetValue(fParameters);
   }
}

void TCatProcessor::PrintDescriptionYAML()
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
         CatProcPrmMap_t::iterator it;
         for (it = fParamMap.begin(); it != fParamMap.end(); it++) {
            TCatParameter *prm = it->second;
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

