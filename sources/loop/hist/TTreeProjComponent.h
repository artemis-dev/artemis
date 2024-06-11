/**
 * @file   TTreeProjComponent.h
 * @brief  Collection of the projections of TTree
 *
 * @date   Created       : 2023-06-26 23:53:58 JST
 *         Last Modified : 2024-06-11 19:30:46 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#ifndef TTREEPROJCOMPONENT_H
#define TTREEPROJCOMPONENT_H

#include "TNamed.h"
#include "TAttCut.h"

namespace art {
   class TTreeProjComponent;
}

class art::TTreeProjComponent : public TNamed {

public:
   TTreeProjComponent();
   TTreeProjComponent(const char* name, const char* title);
   virtual ~TTreeProjComponent();
   
   
   virtual TObject* Clone(const char* opt = "") const {;} // clone method
   virtual void Copy(TObject &obj) const;
   virtual bool IsGroup() { return false; }

   virtual void SetParent(TTreeProjComponent *parent) { fParent = parent; }
   virtual TTreeProjComponent* GetParent() const { return fParent; }

protected:

   TTreeProjComponent *fParent; //! parent component

   ClassDef(TTreeProjComponent,1) // tree projectiom component

};



#endif

