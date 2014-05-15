/**
 * @file   TParameterArrayLoader.h
 * @brief  parameter array loader.
 *         Each line in input file corresponding to a parameter object.
 *         Requires LoadString method in output class,
 *         which inherits from art::TParameterObject.
 *
 * @date   Created       : 2014-03-03 10:54:10 JST
 *         Last Modified : 2014-03-03 11:43:26 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9EDF8D85_FE25_4D05_9552_257C955BB55D
#define INCLUDE_GUARD_UUID_9EDF8D85_FE25_4D05_9552_257C955BB55D

#include "TProcessor.h"

namespace art {
   class TParameterArrayLoader;
}

class TClonesArray;

class art::TParameterArrayLoader : public TProcessor {
public:
   TParameterArrayLoader();
   virtual ~TParameterArrayLoader();

   void Init(TEventCollection *col);

protected:
   TString fParameterArrayName;
   TString fTypeName;
   TString fFileName;
   TString fFileFormat;
   TClonesArray *fParameterArray;

   // loader for text file
   virtual Bool_t LoadText();
   // loader for yaml file
   virtual Bool_t LoadYAML();

private:
   TParameterArrayLoader(const TParameterArrayLoader&); // prohibited
   TParameterArrayLoader& operator=(const TParameterArrayLoader&); // prohibited

   ClassDef(TParameterArrayLoader,0) // parameter array loader
};

#endif // INCLUDE_GUARD_UUID_9EDF8D85_FE25_4D05_9552_257C955BB55D
