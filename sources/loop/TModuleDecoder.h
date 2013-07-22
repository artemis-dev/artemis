/* $Id:$ */
/**
 * @file   TModuleDecoder.h
 * @date   Created : Jul 20, 2013 10:20:00 JST
 *   Last Modified : 
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
   TModuleDecoder(Int_t id, TClass *dataClass = NULL);
   ~TModuleDecoder();

   virtual Int_t ID() { return fID; }
   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg) = 0;

protected:
   virtual TObject* New() { return (fHits)?fHits->ConstructedAt(fHits->GetEntriesFast()):NULL; }
   virtual void     Clear(const Option_t *opt=NULL) { if (fHits) fHits->Clear("C"); }

protected:
   Int_t fID;
   TClonesArray *fHits;
};
#endif // end of #ifdef TMODULEDECODER_H
