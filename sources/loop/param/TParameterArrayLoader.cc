/**
 * @file   TParameterArrayLoader.cc
 * @brief  parameter array loader
 *
 * @date   Created       : 2014-03-03 11:11:54 JST
 *         Last Modified : Apr 29, 2014 13:22:55 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>, Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TParameterArrayLoader.h"
#include <TClonesArray.h>
#include <TClass.h>
#include <TParameterObject.h>
#include <fstream>
#include <yaml-cpp/yaml.h>

using art::TParameterArrayLoader;

ClassImp(TParameterArrayLoader)

namespace {
   const TString DEFAULT_NAME("parameter");
   const TString DEFAULT_TYPE("TParameterObject");
   const TString DEFAULT_FILENAME("path/to/file");
   const TString FORMAT_TEXT("text");
   const TString FORMAT_YAML("yaml");
   const TString DEFAULT_FILE_FORMAT(FORMAT_TEXT);
}

TParameterArrayLoader::TParameterArrayLoader()
: fParameterArray(NULL) {
   RegisterProcessorParameter("Name","name of parameter array output",
			      fParameterArrayName, DEFAULT_NAME);
   RegisterProcessorParameter("Type","type(class) of parameter",
			      fTypeName, DEFAULT_TYPE);
   RegisterProcessorParameter("FileName","input filename",
			      fFileName, DEFAULT_FILENAME);
   RegisterProcessorParameter("FileFormat","file format : text (default), yaml",
			      fFileFormat, DEFAULT_FILE_FORMAT);
}

TParameterArrayLoader::~TParameterArrayLoader() {
   delete fParameterArray;
}

void TParameterArrayLoader::Init(TEventCollection *col) {

   Info("Init","%s %s (%s)",fTypeName.Data(),fParameterArrayName.Data(),fFileName.Data());

   TClass *cl = TClass::GetClass(fTypeName.Data());
   if (!cl) {
      Error("Init","no such type registered in dictionary: %s", fTypeName.Data());
      return;
   }

   if (!cl->InheritsFrom(TParameterObject::Class())) {
      Error("Init","%s does not inherits from art::TParameterObject", fTypeName.Data());
      return;
   }
   // create parameter array
   fParameterArray = new TClonesArray(fTypeName.Data());
   Bool_t succeeded = kFALSE;
   if (fFileFormat == FORMAT_TEXT) {
      if (LoadText()) succeeded = kTRUE;
   } else if (fFileFormat == FORMAT_YAML) {
      if (LoadYAML()) succeeded = kTRUE;
   }
   
   if (!succeeded) {
      Error("Init","No parameter loaded from %s", fFileName.Data());
      delete fParameterArray;
      fParameterArray = NULL;
      return;
   }



   fParameterArray->SetName(fParameterArrayName);
   col->AddInfo(fParameterArrayName,fParameterArray,fOutputIsTransparent);
}

Bool_t TParameterArrayLoader::LoadText()
{
   ifstream fin(fFileName.Data());
   if (!fin.is_open()) {
      Error("Init", "Cannot open file: %s", fFileName.Data());
      return kFALSE;
   }
   TParameterObject *parameter = NULL;
   TString buf;
   Int_t count = 0;
   while(buf.ReadLine(fin)) {
      if (!parameter) {
         parameter =
            static_cast<TParameterObject*>(fParameterArray->ConstructedAt(count));
      }
      
      if (parameter->LoadString(buf)) {
         parameter = NULL;
         ++count;
      }
   }
   
   fin.close();
   
   if(!count) {
      return kFALSE;
   }
   
   fParameterArray->Expand(count);
   Info("Init","%d parameter%s loaded from %s",
        count, count == 1 ? "" : "s", fFileName.Data());
   return kTRUE;
}

Bool_t TParameterArrayLoader::LoadYAML()
{
   ifstream fin(fFileName.Data());
   if (!fin.is_open()){
      Error("LoadYAML","Cannot open file: %s",fFileName.Data());
      return kFALSE;
   }
   YAML::Node doc;
   try {
      YAML::Parser parser(fin);
      parser.GetNextDocument(doc);
      std::string name;

      // parse first document
      for (YAML::Iterator it = doc.begin(); it != doc.end(); it++) {
         it.first() >> name;
         TParameterObject *prm = static_cast<TParameterObject*>(fParameterArray->ConstructedAt(fParameterArray->GetEntriesFast()));
         prm->SetName(name.c_str());
         if (!prm->LoadYAMLNode(it.second())) {
            fParameterArray->Delete();
            Error("LoadYAML","Error while parsing YAML for %s\n",name.c_str());
            return kFALSE;
         }
         printf("%s\n",name.c_str());
      }
   } catch (YAML::Exception& e) {
      Error("LoadYAML","Error occurred while parsing YAML file: %s",fFileName.Data());
      fprintf(stderr,"%s\n",e.what());
      return kFALSE;
   }
   if (!fParameterArray->GetEntriesFast()) return kFALSE;
   return kTRUE;
}


