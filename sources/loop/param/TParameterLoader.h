/**
 * @file   TParameterLoader.h
 * @brief  load parameter from file
 *
 * @date   Created:       2014-02-27 13:50:47
 *         Last Modified: 2014-03-03 10:56:10 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef TPARAMETERLOADER_H
#define TPARAMETERLOADER_H

#include <TProcessor.h>

namespace art {
   class TParameterLoader;
   class TParameterObject;
}

class art::TParameterLoader : public TProcessor {
public:
   // Default constructor
   TParameterLoader();
   virtual ~TParameterLoader();

   void Init(TEventCollection *col);

protected:
   TString           fParameterName;
   TString           fTypeName;
   TString           fFileName;
   TParameterObject *fParameter; //!->

private:
   // Copy constructor (prohibited)
   TParameterLoader(const TParameterLoader&);
   // Assignment operator (prohibited)
   TParameterLoader& operator=(const TParameterLoader&);

   ClassDef(TParameterLoader,1) // parameter loader
};

#endif // TPARAMETERLOADER_H
