/* $Id:$ */
/**
 * @file   TEventCollection.h
 * @date   Created : Apr 26, 2012 23:26:19 JST
 *   Last Modified : Feb 02, 2013 22:01:51 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TEVENTCOLLECTION_H
#define ART_TEVENTCOLLECTION_H

#include <TEventObject.h>

#include <TList.h>

namespace art {
   class TEventCollection;
}

class art::TEventCollection  {

public:

   TEventCollection();

   virtual ~TEventCollection();

   virtual void   Add(const char* name, TObject* obj, Bool_t isPassive = kTRUE);
   virtual TEventObject* Get(const char* name);
   virtual TIter* GetIter() { return new TIter(fObjects); }
   
protected:
   TList *fObjects;
};


#endif // end of #ifdef TCATEVENTCOLLECTION_H
