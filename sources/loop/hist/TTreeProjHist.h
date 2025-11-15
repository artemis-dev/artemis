/**
 * @file   TTreeProjHist.h
 * @brief  Histgram difinition
 *
 * @date   Created       : 2023-06-28 00:30:27 JST
 *         Last Modified : 2024-06-11 19:53:05 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#ifndef TTREEPROJHIST_H
#define TTREEPROJHIST_H

#include "TTreeProjComponent.h"
#include "TAttTreeProj.h"

namespace art {
   class TTreeProjHist;
   class TTreeProjGroup;
   template <class TH> class TTreeProjHistImpl;
}
class TTree;

class art::TTreeProjHist : public TTreeProjComponent, public TAttTreeProj {
public:
   TTreeProjHist(const char* name, const char* title, TH1* hist, const char* cut = "");

   virtual void Copy (TObject& obj) const;

protected:
   ClassDef(TTreeProjHist,1) // tree projection hist base class

};


template <class TH>
class art::TTreeProjHistImpl : public  TTreeProjHist {

public:
   TTreeProjHistImpl() 
      : TTreeProjHist("tmp","tmp",new TH, "")
      {
      }
         
   TTreeProjHistImpl(const char *name, const char* title, const char* cut)
      : TTreeProjHist(name,title,new TH,cut)
      {
      }
   virtual TObject* Clone(const char* opt = "") const {
      TObject *histobj = new art::TTreeProjHistImpl<TH>;
      Copy(*histobj);
      return histobj;
   }      

//   ClassDef(TTreeProjHist,1) // tree projectiom hist

};


#endif


