/* $Id:$ */
/**
 * @file   TArtemisObject.h
 * @date   Created : Dec 01, 2013 03:01:54 JST
 *   Last Modified : Jan 07, 2014 11:29:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TARTEMISOBJECT_H
#define TARTEMISOBJECT_H

#include <TObject.h>

namespace art {
   class TArtemisObject;
}

class art::TArtemisObject  : public TObject {

public:
   TArtemisObject();
   TArtemisObject(const TArtemisObject &obj);
   virtual ~TArtemisObject();

   ClassDef(TArtemisObject,1); // base of artemis objects
};
#endif // end of #ifdef TARTEMISOBJECT_H
