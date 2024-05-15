/* $Id:$ */
/**
 * @file   TModuleDecoderRCNP.h
 * @date   Created : Jul 20, 2013 10:20:00 JST
 *   Last Modified : 2024-03-23 18:51:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODERRCNP_H
#define TMODULEDECODERRCNP_H

#include <TNamed.h>
#include <TClonesArray.h>
class TObjArray;
namespace art {
   class TModuleDecoderRCNP;
}


class art::TModuleDecoderRCNP  : public TNamed {

public:
   TModuleDecoderRCNP();
   TModuleDecoderRCNP(Int_t id, TClass *dataClass = NULL);
   ~TModuleDecoderRCNP();

   // override default decoder id
   virtual void SetID(Int_t id) { fID = id; }
   virtual Int_t GetID() { return fID; }
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg) = 0;
   virtual void SetClass(TClass *dataClass);
   virtual TClass* GetClass() { return fHits?fHits->GetClass():NULL; }

   virtual void Copy(TObject& obj) const;

protected:
   virtual TObject* New() {
	  printf("nHits=%d\n",fHits->GetEntriesFast());
	  return (fHits)?fHits->ConstructedAt(fHits->GetEntriesFast()):NULL; 
	}
	void Size(){
	  printf("nHits=%d\n",fHits->GetEntriesFast());
	}
   virtual void     Clear(const Option_t *opt=NULL) { if (fHits) fHits->Clear("C"); }

protected:
   Int_t fID;
   TClonesArray *fHits;

   ClassDef(TModuleDecoderRCNP,1); // base class of module decoder
};
#endif // end of #ifdef TMODULEDECODERRCNP_H
