/* $Id:$ */
/**
 * @file   TEventCollection.h
 * @date   Created : Apr 26, 2012 23:26:19 JST
 *   Last Modified : 2020-11-27 00:11:40 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TEVENTCOLLECTION_H
#define ART_TEVENTCOLLECTION_H

#include "TList.h"
#include "TEventObject.h"
namespace art {
   class TEventCollection;

}

class art::TEventCollection  {

public:

   TEventCollection();

   virtual ~TEventCollection();

   // add an event object
   virtual void Add(const char* name, TObject* obj, Bool_t isPassive = kTRUE);
   virtual void Add(TEventObject *obj) { fObjects->Add(obj); }
   // add a information object
   virtual void AddInfo(const char *name, TObject *obj, Bool_t isPassive = kFALSE);
   // get an event object by name
   virtual TEventObject* Get(const char* name);
   // get a reference to an event object by name
   virtual void** GetObjectRef(const char *name);
   // get an information object
   virtual TObject* GetInfo(const char *name);
   // get an information object
   virtual void** GetInfoRef(const char *name);
   // get the iterator of event objects
   virtual TIter* GetIter() { return new TIter(fObjects); }
   // get the list of information objects
   virtual TIter* GetUserInfoIter() { return new TIter(fUserInfo); }
   virtual void Delete();
   
protected:
   TList *fObjects; //! list of object
   TList *fUserInfo; //! list of user objects associated to this process

   ClassDef(TEventCollection,1); // Event collection containing the data container 
};


#endif // end of #ifdef TCATEVENTCOLLECTION_H
