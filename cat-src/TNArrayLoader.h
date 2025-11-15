/**
 * @file   TNArrayLoader.h
 * @brief  load TNArray
 *
 * @date   Created       : 2018-04-26 11:59:03 JST
 *         Last Modified : 2018-04-26 15:37:36 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_B400E9DF_9D38_4CB9_841C_ADE04681CA27
#define INCLUDE_GUARD_UUID_B400E9DF_9D38_4CB9_841C_ADE04681CA27

#include "TProcessor.h"

namespace art {
   class TNArrayLoader;
   class TNArray;
}
class TFile;

class art::TNArrayLoader : public TProcessor {
public:
   TNArrayLoader();
   virtual ~TNArrayLoader();

   TNArrayLoader(const TNArrayLoader& rhs);
   TNArrayLoader& operator=(const TNArrayLoader& rhs);

   virtual void Init(TEventCollection *col);


protected:
   TString fFileName;
   TString fArrayName;
   TString fOutputName;

   TFile *fFile; //!
   TNArray *fArray; //!

   

private:

   ClassDef(TNArrayLoader,1) // load TNArray
};

#endif // INCLUDE_GUARD_UUID_B400E9DF_9D38_4CB9_841C_ADE04681CA27
