/**
 * @file   TConverterUtil.cc
 * @brief
 *
 * @date   Created:       2013-11-08 09:49:21
 *         Last Modified: 2014-03-04 17:03:12 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TConverterUtil.h"
#include "TConverterBase.h"
#include <TEventCollection.h>
#include <TClonesArray.h>
#include <TString.h>
#include <TClass.h>

using art::TConverterUtil;

void TConverterUtil::SetConverterArray(std::vector<TConverterBase*> **p,
				       const TString &name,
				       TEventCollection* const col)
{
   if(!p) return;
   delete *p;

   TClonesArray *cArray =
      dynamic_cast<TClonesArray*>(col->GetInfo(name.Data()));
   if(!cArray) return;

   TClass *cl = cArray->GetClass();
   if(!cl->InheritsFrom(TConverterBase::Class())) {
      fprintf(stderr,"Error: %s does not inherits from art::TConverterBase.\n",
	 name.Data());
   }

   *p = new std::vector<TConverterBase*>;

   Int_t nDet = cArray->GetEntriesFast();
   (*p)->reserve(nDet);
   for(Int_t iDet=0;iDet!=nDet;++iDet) {
      (*p)->push_back(static_cast<TConverterBase*>(cArray->At(iDet)));
   }
}

void TConverterUtil::SetConverterArrayFromList(std::vector<TConverterBase*> **p,
					       const StringVec_t &nameList,
					       TEventCollection* const col)
{
   if(!p) return;

   delete *p;
   *p = new std::vector<TConverterBase*>;

   const Int_t size = nameList.size();
   (*p)->reserve(size);

   for(Int_t i = 0; i != size; ++i) {
      TConverterBase *const conv =
	 dynamic_cast<TConverterBase*>(col->GetInfo(nameList[i].Data()));
      if(!conv) {
	 delete *p;
	 *p = NULL;
	 return;
      }
      (*p)->push_back(conv);
   }
}
