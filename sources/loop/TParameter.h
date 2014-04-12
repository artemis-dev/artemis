/* $Id:$ */
/**
 * @file   TParameter.h
 * @date   Created : May 18, 2012 14:18:34 JST
 *   Last Modified : 2014-04-12 17:00:33 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2012
 */
#ifndef ART_TPARAMETER_H
#define ART_TPARAMETER_H


#include <vector>

#include <TString.h>

#include <TParameterStrings.h>
#include <TTypes.h>

namespace art {
   class TParameter;
   template <class T> class TParameter_t;
}

class art::TParameter {

public:
   TParameter(const char* name, const char* title)
      : fName(name), fTitle(title) {
   }

   virtual ~TParameter() { ;}

   virtual TString GetName() const { return fName; }
   virtual TString GetTitle() const { return fTitle; }

   virtual void SetValue(TParameterStrings *p) = 0;
   virtual TString Type() const = 0;

   virtual TString DefaultValue() const = 0;
   virtual TString Value() const = 0;

   virtual Bool_t IsVector() const = 0;

   virtual Bool_t IsOptional() const { return fIsOptional; }

   virtual Bool_t IsValueSet() const { return fIsValueSet; }

   virtual Int_t Size() const { return fSize; }

protected:
   TParameter() { ; }
   TString fName;
   TString fTitle;
   Bool_t fIsOptional;
   Bool_t fIsValueSet;
   Int_t  fSize;
};

namespace art {
   template <class T>
   class TParameter_t : public TParameter {
   public:
      TParameter_t(const char *name, const char* title,
                   T& param, const T& defval, bool isopt, int size=0 )
         : TParameter(name,title), fParameter(param), fDefValue(defval)
         {
            fParameter = fDefValue;
            fIsOptional = isopt;
            fIsValueSet = kFALSE;
	    fSize = size;
         }

      virtual ~TParameter_t() { }

      virtual TString Type() const{
         // not implemented
         if (typeid(T) == typeid(Int_t)) return "Int_t";
         if (typeid(T) == typeid(Long_t)) return "Long_t";
         if (typeid(T) == typeid(Float_t)) return "Float_t";
         if (typeid(T) == typeid(Double_t)) return "Double_t";
         if (typeid(T) == typeid(Bool_t)) return "Bool_t";
         if (typeid(T) == typeid(TString)) return "TString";
         if (typeid(T) == typeid(IntVec_t)) return "IntVec_t";
         if (typeid(T) == typeid(FloatVec_t)) return "FloatVec_t";
         if (typeid(T) == typeid(std::vector<Double_t>)) return "vector<Double_t>";
         if (typeid(T) == typeid(std::vector<Bool_t>)) return "vector<Bool_t>";
         if (typeid(T) == typeid(StringVec_t)) return "StringVec_t";
         return "unsupported";
      }

      virtual Bool_t IsVector() const {
	 if (typeid(T) == typeid(IntVec_t)) return kTRUE;
	 if (typeid(T) == typeid(FloatVec_t)) return kTRUE;
	 if (typeid(T) == typeid(std::vector<Double_t>)) return kTRUE;
	 if (typeid(T) == typeid(std::vector<Bool_t>)) return kTRUE;
	 if (typeid(T) == typeid(StringVec_t)) return kTRUE;
	 return kFALSE;
      }

      void SetValue(TParameterStrings *param) {
         if (param->IsSet(GetName())) {
            param->GetValue(GetName(),fParameter);
            fIsValueSet = kTRUE;
         }
      }

      TString DefaultValue() const { return ToString(fDefValue,fSize); }
      TString Value() const { return ToString(fParameter,fSize); }


      T &fParameter;
      T  fDefValue;

      template<class T1>
      static TString ToString(std::vector<T1> v, int) {
	 TString str;
         typename std::vector<T1>::iterator it;
         for (it = v.begin(); it != v.end(); it++) {
            str += *it;
            if (it + 1 != v.end()) {
               str += ", ";
            }
         }
	 return str;
      }

      static TString ToString(Int_t prm, int) {
         TString str;
         str += prm;
         return str;
      }
      static TString ToString(Long_t prm, int) {
         TString str;
         str += prm;
         return str;
      }
      static TString ToString(Float_t prm, int) {
         TString str;
         str += prm;
         return str;
      }
      static TString ToString(TString prm, int) {
         TString str;
         str += prm;
         return str;
      }

//   ClassDef(TParameter_t,1);
   };


   typedef TParameter_t<Int_t> TParameterI;
   typedef TParameter_t<Float_t> TParameterF;
   typedef TParameter_t<TString> TParameterS;
   typedef TParameter_t<IntVec_t> TParameterArrayI;
   typedef TParameter_t<FloatVec_t> TParameterArrayF;
   typedef TParameter_t<StringVec_t> TParameterArrayS;
}
#endif // end of #ifdef TCATPARAMETER_H
