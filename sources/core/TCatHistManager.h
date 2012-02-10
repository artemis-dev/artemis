/* $Id:$ */
/**
 * @file   TCatHistManager.h
 * @date   Created : Feb 06, 2012 11:06:32 JST
 *   Last Modified : Feb 10, 2012 00:29:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATHISTMANAGER_H
#define TCATHISTMANAGER_H

#include <TObject.h>
#include <TDirectory.h>

class TCatHistManager  : public TObject {
protected:
   TCatHistManager();
public:
   ~TCatHistManager();

   static TCatHistManager *Instance();

   virtual void Load();
   virtual void DrawNext(const Option_t *opt = "");
   virtual void DrawCurrent(const Option_t *opt = "");
   virtual void DrawPrev(const Option_t *opt = "");
   virtual void DrawObject(const Int_t &id, const Option_t *opt = "");
   
   virtual TObject* GetObject(const char* name);
   virtual TObject* GetObject(const Int_t &id);
   virtual TObject* GetCurrent();
   virtual TObject* GetNext();
   virtual TObject* GetPrev();

   virtual void SetId(const Int_t &id);
   virtual Int_t GetId() { return fCurrentID; }

   virtual void ls(Option_t * /*opt*/="") const;
   virtual Bool_t cd(TString /*opt*/) const;
   virtual Bool_t cd(const Int_t &id) const;
private:
   Int_t  fCurrentID;
   TList *fObjs; //->
   TList *fKeys; //->
   ClassDef(TCatHistManager,1);
};
#endif // end of #ifdef TCATHISTMANAGER_H
