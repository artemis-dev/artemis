/**
 * @file   TMultiFileParameterArrayLoader.h
 * @brief  parameter array loader for multiple files
 *
 * @date   Created       : 2015-04-20 12:52:05 JST
 *         Last Modified : 2018-02-04 11:57:20 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_C090623D_CE19_4E74_B5E4_6BE6F1AE4220
#define INCLUDE_GUARD_UUID_C090623D_CE19_4E74_B5E4_6BE6F1AE4220

#include "TProcessor.h"

namespace art {
   class TMultiFileParameterArrayLoader;
}

class TClonesArray;

class art::TMultiFileParameterArrayLoader : public TProcessor {
public:
   TMultiFileParameterArrayLoader();
   virtual ~TMultiFileParameterArrayLoader();

   void Init(TEventCollection *col);

protected:
   TString       fParameterArrayName;
   TString       fTypeName;
   StringVec_t   fFileList;
   TClonesArray *fParameterArray; //!

private:
   TMultiFileParameterArrayLoader(const TMultiFileParameterArrayLoader&);
   TMultiFileParameterArrayLoader& operator=(const TMultiFileParameterArrayLoader&);

   ClassDef(TMultiFileParameterArrayLoader,0) // parameter array loader for multiple files
};

#endif // INCLUDE_GUARD_UUID_C090623D_CE19_4E74_B5E4_6BE6F1AE4220
