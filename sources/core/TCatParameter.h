/* $Id:$ */
/**
 * @file   TCatParameter.h
 * @date   Created : May 18, 2012 14:18:34 JST
 *   Last Modified : May 18, 2012 15:53:28 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPARAMETER_H
#define TCATPARAMETER_H


#include <vector>

#include <TString.h>

#include <TCatParameterStrings.h>
#include <TCatTypes.h>

class TCatParameter {

public:
   TCatParameter(const char* name, const char* title)
      : fName(name), fTitle(title) {
      
   }


   virtual ~TCatParameter() { ;}

   virtual TString GetName() { return fName; }
   virtual TString GetTitle() { return fTitle; }
   

   virtual void SetValue(TCatParameterStrings *p) = 0;

   virtual Bool_t IsOptional() { return fIsOptional; }

   virtual Bool_t IsValueSet() { return fIsValueSet; }

   virtual Int_t Size() { return fSize; }

protected:
   TCatParameter() { ; }
   TString fName;
   TString fTitle;
   Bool_t fIsOptional;
   Bool_t fIsValueSet;
   Int_t  fSize;
};

template <class T>
class TCatParameter_t : public TCatParameter {
public:
   TCatParameter_t(const char *name, const char* title,
                   T& param, const T& defval, bool isopt, int size=0 )
      : TCatParameter(name,title), fParameter(param), fDefValue(defval)
      {
         fParameter = fDefValue;
         fIsOptional = isopt;
         fIsValueSet = kFALSE;
         fSize = size;
      }

   virtual ~TCatParameter_t() { }

   virtual const TString Type() {
      // not implemented
      return "type";
   }

   void SetValue(TCatParameterStrings *param) {
      if (param->IsSet(GetName())) {
         param->GetValue(GetName(),*fParameter);
         fIsValueSet = kTRUE;
      }
   }
   T &fParameter;
   T  fDefValue;
//   ClassDef(TCatParameter_t,1);
};

typedef TCatParameter_t<Int_t> TCatParameterI;
typedef TCatParameter_t<Float_t> TCatParameterF;
typedef TCatParameter_t<TString> TCatParameterS;
typedef TCatParameter_t<IntVec_t> TCatParameterArrayI;
typedef TCatParameter_t<FloatVec_t> TCatParameterArrayF;
typedef TCatParameter_t<StringVec_t> TCatParameterArrayS;

#endif // end of #ifdef TCATPARAMETER_H
