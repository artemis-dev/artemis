/* $Id:$ */
/**
 * @file   TEventObject.h
 * @date   Created : Jul 10, 2013 14:10:50 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TEVENTOBJECT_H
#define ART_TEVENTOBJECT_H

#include <TNamed.h>

namespace art {
   class TEventObject;
}

class art::TEventObject  : public TNamed {

public:
   /*
    * default constructor
    */
   TEventObject();
   /*
    * Constructor to hold a object which inherits from TObject.
    *
    * @param name the name of the object
    * @param obj  pointer to the TObject
    * @param isPassive flag to fill the object to output tree
    *
    */
   TEventObject(const char* name,TObject *obj, Bool_t isPassive = kTRUE);
   ~TEventObject();

   void**  GetObjectRef() { return &fP; }
   Bool_t  IsPassive() { return fIsPassive; }
   TClass *GetClass() { return fClass; }
private:
   void   *fP;
   Bool_t  fIsPassive;
   TClass *fClass;

   ClassDef(TEventObject,1);
};
#endif // end of #ifdef TEVENTOBJECT_H
