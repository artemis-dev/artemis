/**
 * @file   TObjectLoader.h
 * @brief  load object from root file
 *
 * @date   Created       : 2018-06-27 18:45:15 JST
 *         Last Modified : 2018-06-27 18:48:19 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_C8E52347_4B27_432C_83ED_1C642880B0E4
#define INCLUDE_GUARD_UUID_C8E52347_4B27_432C_83ED_1C642880B0E4

#include "TProcessor.h"

namespace art {
   class TObjectLoader;
}

class art::TObjectLoader : public TProcessor {
public:
   TObjectLoader();
   virtual ~TObjectLoader();

   TObjectLoader(const TObjectLoader& rhs);
   TObjectLoader& operator=(const TObjectLoader& rhs);

   virtual void Init(TEventCollection *);

protected:
   StringVec_t fObjectLists; // name of objects to be loaded
   std::vector<TObject*> fObjects;  //!

private:

   ClassDef(TObjectLoader,1) // load object from root file
};

#endif // INCLUDE_GUARD_UUID_C8E52347_4B27_432C_83ED_1C642880B0E4
