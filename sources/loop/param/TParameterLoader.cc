/**
 * @file   TParameterLoader.cc
 * @brief
 *
 * @date   Created:       2014-02-27 13:59:19
 *         Last Modified: 2014-03-13 11:16:50 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TParameterLoader.h"
#include "TParameterObject.h"

#include <TClass.h>

using art::TParameterLoader;

ClassImp(TParameterLoader)

namespace {
   const TString DEFAULT_NAME("parameter");
   const TString DEFAULT_TYPE("art::TParameterObject");
   const TString DEFAULT_FILENAME("path/to/file");
}

TParameterLoader::TParameterLoader()
   : fParameter(NULL) {
   RegisterProcessorParameter("Name","name of parameter output",
			      fParameterName, DEFAULT_NAME);
   RegisterProcessorParameter("Type","type(class) of parameter",
			      fTypeName, DEFAULT_TYPE);
   RegisterProcessorParameter("FileName","input filename",
			      fFileName, DEFAULT_FILENAME);
}

TParameterLoader::~TParameterLoader() {
   delete fParameter;
}

void TParameterLoader::Init(TEventCollection *col) {

   Info("Init","%s %s (%s)",fTypeName.Data(),fParameterName.Data(),fFileName.Data());

   TClass *cl = TClass::GetClass(fTypeName.Data());
   if (!cl) {
      Error("Init","no such type registered in dictionary: %s", fTypeName.Data());
      return;
   }

   if (!cl->InheritsFrom(TParameterObject::Class())) {
      Error("Init","%s does not inherits from art::TParameterObject", fTypeName.Data());
      return;
   }

   fParameter = static_cast<TParameterObject*>(cl->New());
   fParameter->SetName(fParameterName);
   if (fParameter->LoadFile(fFileName)) {
      fParameter->Print();
      col->AddInfo(fParameterName,fParameter,fOutputIsTransparent);
   } else {
      SetStateError(TString::Format("Could not load file %s",fFileName.Data()));
      delete fParameter;
      fParameter = NULL;
   }
}

