/* $Id:$ */
/**
 * @file   TModuleDecoder.h
 * @date   Created : Jul 20, 2013 10:20:00 JST
 *   Last Modified : May 12, 2014 23:26:04 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMODULEDECODER_H
#define TMODULEDECODER_H

#include <TNamed.h>
#include <TClonesArray.h>
class TObjArray;
namespace art {
   class TModuleDecoder;
}


class art::TModuleDecoder  : public TNamed {

public:
   TModuleDecoder();
   TModuleDecoder(Int_t id, TClass *dataClass = NULL);
   ~TModuleDecoder();

   // override default decoder id
   virtual void SetID(Int_t id) { fID = id; }
   virtual Int_t GetID() { return fID; }
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg) = 0;
   virtual void SetClass(TClass *dataClass);
   virtual TClass* GetClass() { return fHits?fHits->GetClass():NULL; }

   virtual void Copy(TObject& obj) const;

protected:
   virtual TObject* New() { return (fHits)?fHits->ConstructedAt(fHits->GetEntriesFast()):NULL; }
   virtual void     Clear(const Option_t *opt=NULL) { if (fHits) fHits->Clear("C"); }

protected:
   Int_t fID;
   TClonesArray *fHits;

   ClassDef(TModuleDecoder,1); // base class of module decoder
};
#endif // end of #ifdef TMODULEDECODER_H
