/**
 * @file   TParameterObject.cc
 * @brief
 *
 * @date   Created:       2014-02-26 16:24:12
 *         Last Modified: 2014-03-05 13:58:41 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TParameterObject.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <TString.h>
#include <TRegexp.h>
#include <TROOT.h>

using art::TParameterObject;

ClassImp(TParameterObject)

TParameterObject::TParameterObject() : fLoadedFileStat(NULL) {}

TParameterObject::~TParameterObject() {
   delete fLoadedFileStat;
}

void TParameterObject::Clear(Option_t*) {
   fLoaded = kFALSE;
   if (fLoadedFileStat) fLoadedFileStat->clear();
}

Bool_t TParameterObject::Loaded(const FileStat_t& fstat) const {
   if (fLoadedFileStat) {
      for (std::list<FileStat_t>::iterator it = fLoadedFileStat->begin(),
	      end = fLoadedFileStat->end();
	   it != end; ++it ) {
	 if (fstat.fIno == it->fIno) {
	    return kTRUE;
	 }
      }
   }
   return kFALSE;
}

Bool_t TParameterObject::Loaded(const TString& filename) const {
   FileStat_t fstat;
   gSystem->GetPathInfo(filename,fstat);
   return Loaded(fstat);
}

Bool_t TParameterObject::LoadFile(const TString& filename)
{
   FileStat_t fstat;
   gSystem->GetPathInfo(filename,fstat);

   if (Loaded(fstat)) {
      Error("LoadFile","File already loaded: %s", filename.Data());
      return kFALSE;
   }

   if (!fLoadedFileStat) {
      fLoadedFileStat = new std::list<FileStat_t>;
   }

   Bool_t succeeded = kFALSE;
   fLoaded = kFALSE;
   if (filename.Contains(TRegexp(".ya?ml"))) {
      succeeded = LoadYAMLFile(filename);
   } else {
      ifstream fin(filename.Data());
      if (!fin.is_open()) {
	 Error("LoadFile", "Cannot open file: %s",filename.Data());
	 return kFALSE;
      }
      succeeded = LoadTextFile(fin);
   }

   if (succeeded) {
      fLoaded = kTRUE;
      fLoadedFileStat->push_back(fstat);
   }
   return succeeded;
}

Bool_t TParameterObject::LoadTextFile(istream&)
{
   // Load Text filestream.
   // Classes that provide text file loading must override this method.

   AbstractMethod("LoadTextFile");
   return kFALSE;
}

Bool_t TParameterObject::LoadYAMLFile(const TString &filename)
{
   // Load YAML file.
   // The root node is used as argument of LoadYAML(const YAML::Node& node).

   ifstream fin(filename.Data());
   if (!fin.is_open()) {
      Error("LoadFile", "Cannot open file: %s",filename.Data());
      fCurrentFile = "";
      return kFALSE;
   }
   fCurrentFile = filename;

   YAML::Node doc;
   try {
      YAML::Parser parser(fin);
      parser.GetNextDocument(doc);
      return LoadYAMLNode(doc);
   } catch (YAML::Exception& e) {
      Error("LoadYAML","Error occurred while parsing YAML file: %s",filename.Data());
      fprintf(stderr,"%s\n",e.what());
      return kFALSE;
   }
}

Bool_t TParameterObject::LoadYAMLNode(const YAML::Node&  node )
{
   // Prints desctiption to stdout.
   // Classes that provide YAML Node loading must override this method.
   node >> &fParameterStrings;
   TParameterStrings::map_t::const_iterator iter = fParameterStrings.Begin();
   TParameterStrings::map_t::const_iterator end = fParameterStrings.End();
   while (iter!=end) {
      if (fParameterMap.find(iter->first) == fParameterMap.end()) {
         Warning("LoadYAMLNode","(%s) Parameter named '%s' is not registered",
                 GetName(),
                 iter->first.Data());
      }
      iter++;
   }
   for (ParameterMap_t::iterator it = fParameterMap.begin(); it != fParameterMap.end(); it++) {
      if (!fParameterStrings.IsSet(it->first)) {
         if (!it->second->IsOptional()) {
            Error("LoadYAMLNode","Parameter '%s' is mandatory but not set in '%s'",it->first.Data(),GetName());
            return kFALSE;
         } else {
            // skip this value
            continue;
         }
      }
      it->second->SetValue(&fParameterStrings);
   }
   return kTRUE;
}

Bool_t TParameterObject::LoadString(const TString& /* string */)
{
   // Load from TString.
   // Classes that provide TString loading must override this method.

   AbstractMethod("LoadString");
   return kFALSE;
}

void TParameterObject::PrintDescriptionText() const
{
   // Prints desctiption to stdout.
   // Classes that provide text file loading should override this method.

   AbstractMethod("PrintDescription");
}

void TParameterObject::PrintDescriptionYAML() const
{
   // Prints desctiption to stdout.
   // Classes that provide YAML Node loading should override this method.

   AbstractMethod("PrintDescriptionYAML");
}

void TParameterObject::Print(Option_t */* opt*/) const
{
   TString outs;
   TString indent(' ');
   for (Int_t i=0; i!=gROOT->GetDirLevel(); i++) {
      indent.Append(' ');
   }
   outs.Append(GetName()).Append("\n");
   for (ParameterMap_t::const_iterator it = fParameterMap.begin(); it != fParameterMap.end(); it++) {
      TProcessorParameter *prm = it->second;
      const TString &value = prm->IsValueSet() ? prm->Value():prm->DefaultValue();
      outs.Append(indent).Append(TString::Format("%-20s",prm->GetName().Data()))
         .Append(": ").Append(value).Append("\n");
   }
   printf("%s",outs.Data());
}
