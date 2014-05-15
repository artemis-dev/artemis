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

#include <TSystem.h>
#include <TNamed.h>

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

protected:
   Bool_t LoadYAMLFile(const TString& filename);
   virtual Bool_t LoadTextFile(istream& /* stream */);

   std::list<FileStat_t> *fLoadedFileStat; //!filestat of loaded file(s) for current parameter
   Bool_t fLoaded; // loaded flag

   ClassDef(TParameterObject,1) // The basis for an artemis parameter object
};

#endif // TPARAMETEROBJECT_H
