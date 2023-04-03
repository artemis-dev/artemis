/**
 * @file   TMultiFileParameterArrayLoader.cc
 * @brief  parameter array loader from multiple files
 *
 * @date   Created       : 2015-04-20 12:54:49 JST
 *         Last Modified : 2015-04-20 13:49:40 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TMultiFileParameterArrayLoader.h"

#include <TClonesArray.h>

#include <TParameterObject.h>

using art::TMultiFileParameterArrayLoader;

ClassImp(TMultiFileParameterArrayLoader)

TMultiFileParameterArrayLoader::TMultiFileParameterArrayLoader()
   : fParameterArray(NULL) {
   RegisterProcessorParameter("Name","name of parameter array",
			      fParameterArrayName, TString("parameter"));
   RegisterProcessorParameter("Type","type(class) of parameter",
			      fTypeName, TString("TParameterObject"));
   RegisterProcessorParameter("FileList","input filename",
			      fFileList, StringVec_t(1,"path/to/file"));

}

TMultiFileParameterArrayLoader::~TMultiFileParameterArrayLoader()
{
   delete fParameterArray;
}

void TMultiFileParameterArrayLoader::Init(TEventCollection *col)
{
   TClass *cl = TClass::GetClass(fTypeName.Data());
   if (!cl) {
      SetStateError(TString::Format("no such type registered in dictionary: %s", fTypeName.Data()));
      return;
   }

   if (!cl->InheritsFrom(TParameterObject::Class())) {
      SetStateError(TString::Format("%s does not inherits from art::TParameterObject", fTypeName.Data()));
      return;
   }

   fParameterArray = new TClonesArray(fTypeName.Data(),1);
   fParameterArray->SetName(fParameterArrayName);

   for (StringVec_t::const_iterator it = fFileList.begin(), end = fFileList.end();
	it != end; ++it) {
      TParameterObject *parameter =
	 static_cast<TParameterObject*>(fParameterArray->ConstructedAt(fParameterArray->GetEntriesFast()));
      if (parameter->LoadFile(*it)) {
	 parameter->Print();
      } else {
	 SetStateError(TString::Format("Could not load file %s",it->Data()));
	 delete fParameterArray;
	 fParameterArray = NULL;
      }
   }

   col->AddInfo(fParameterArrayName,fParameterArray,fOutputIsTransparent);
}
