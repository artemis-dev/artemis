/* $Id:$ */
/**
 * @file   TEventObject.h
 * @date   Created : Jul 10, 2013 14:10:50 JST
 *   Last Modified : 2020-12-02 17:38:45 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TEVENTOBJECT_H
#define ART_TEVENTOBJECT_H

#include <TNamed.h>
#include <TClass.h>
namespace art {
   class TEventObject;
}

class TTree;

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
   // constructor to hold primitive type
   TEventObject(const char* name,void* obj, TString type, TString *length, Bool_t isPassive = kTRUE);

   ~TEventObject();

   void**  GetObjectRef() { return &fP; }
   Bool_t  IsPassive() { return fIsPassive; }
   TClass *GetClass() { return fClass; }
   const char*  GetType() { return fType.Data(); }
   Bool_t IsDouble() { return (fType == "D"); }
   Bool_t IsFloat() { return (fType == "F"); }
   Bool_t IsInt() { return (fType == "I"); }
   TString GetLength() { return (fLength)?*fLength:TString(""); }
   Bool_t IsObject() { return fIsObject; }

   Bool_t SetBranch(TTree *tree,Int_t splitLevel = 0);
      

private:
   void   *fP; //![1]
   Bool_t  fIsPassive; //
   TClass *fClass; //!
   TString fType; //
   TString *fLength; //!
   Bool_t fIsObject; //


   ClassDef(TEventObject,1);
};
#endif // end of #ifdef TEVENTOBJECT_H
