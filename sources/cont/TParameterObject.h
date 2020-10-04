/**
 * @file   TParameterObject.h
 * @brief
 *
 * @date   Created:       2014-02-26 15:40:47
 *         Last Modified: 2014-03-05 13:08:53 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef TPARAMETEROBJECT_H
#define TPARAMETEROBJECT_H

#include <list>
#include <map>

#include <TSystem.h>
#include <TNamed.h>
#include <iostream>

#include "TProcessorParameter.h"
#include "TParameterStrings.h"


namespace art {
   class TParameterObject;
}

namespace YAML {
   class Node;
}

class TRegexp;
class art::TParameterObject : public TNamed {
public:
   TParameterObject();
   virtual ~TParameterObject();

   Bool_t LoadFile(const TString& filename);
   virtual Bool_t LoadString(const TString& /* string */);
   virtual Bool_t LoadYAMLNode(const YAML::Node& /* node */);
   virtual void PrintDescriptionYAML() const;
   virtual void PrintDescriptionText() const;

   virtual Bool_t Loaded() const {return fLoaded;}
   virtual Bool_t Loaded(const TString& filename) const;
   virtual Bool_t Loaded(const FileStat_t& fstat) const;

   virtual void Clear(Option_t *opt="");
   virtual const char* GetCurrentFile() { return fCurrentFile.Data(); }

   virtual void Print(Option_t *opt = "") const;

protected:
   Bool_t LoadYAMLFile(const TString& filename);
   virtual Bool_t LoadTextFile(std::istream& /* stream */);
   std::list<FileStat_t> *fLoadedFileStat; //!filestat of loaded file(s) for current parameter
   Bool_t fLoaded; // loaded flag
   TString fCurrentFile; //! current file name


   // autoload functional
   template <class T>
   void RegisterParameter(const char* name, const char* description,
                          T& parameter, const T& defaultParameter, Int_t size) {
      fParameterMap[TString(name)] =
         new TParameter_t<T>(name,description,parameter,defaultParameter,false,size);
   }

   template <class T>
   void RegisterOptionalParameter(const char* name, const char* description,
                          T& parameter, const T& defaultParameter, Int_t size) {
      fParameterMap[TString(name)] =
         new TParameter_t<T>(name,description,parameter,defaultParameter,true,size);
   }

   typedef std::map<TString,TProcessorParameter*> ParameterMap_t;
   ParameterMap_t fParameterMap; //!
   TParameterStrings fParameterStrings; //!
   

   ClassDef(TParameterObject,1) // The basis for an artemis parameter object
};

#endif // TPARAMETEROBJECT_H
