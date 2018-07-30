/**
 * @file   TProcessorParameter.h
 * @brief  processor parameter
 *
 * @date   Created       : 2014-05-09 18:06:39 JST
 *         Last Modified : 2018-07-30 08:39:55 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_6D72126A_3ED4_434A_BA27_441B5C89DC2E
#define INCLUDE_GUARD_UUID_6D72126A_3ED4_434A_BA27_441B5C89DC2E

#include <TParameterStrings.h>
#include <TNamed.h>

namespace art {
   class TProcessorParameter;
   template <class T> class TParameter_t;
}

class art::TProcessorParameter  : public TNamed {
public:
   TProcessorParameter(const char* name, const char* title)
      : TNamed(name,title)
      {   
      }
   
   virtual ~TProcessorParameter(){;}
   
   virtual void SetValue(TParameterStrings *p) = 0;
   virtual TString Type() const = 0;
   
   virtual TString DefaultValue() = 0;
   virtual TString Value() = 0;

   virtual Bool_t IsStringVector() const = 0;
   virtual Bool_t IsVector() const = 0;
   
   virtual Bool_t IsOptional() { return fIsOptional; }
   
   virtual Bool_t IsValueSet() { return fIsValueSet; }
   
   virtual Int_t Size() { return fSize; }
   
protected:
   TProcessorParameter(){;}
   Bool_t fIsOptional;
   Bool_t fIsValueSet;
   Int_t  fSize;
   
private:

   ClassDef(TProcessorParameter,1); // 
   
};

namespace art {
template <class T>
class TParameter_t : public TProcessorParameter {

public:
   TParameter_t(const char *name, const char* title,
                T& param, const T& defval, bool isopt, int size=0 )
      : TProcessorParameter(name,title), fParameter(param), fDefValue(defval)
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
   
   virtual Bool_t IsStringVector() const {
      if (typeid(T) == typeid(StringVec_t)) return kTRUE;
      return kFALSE;
   }
   
   virtual Bool_t IsVector() const {
      if (typeid(T) == typeid(IntVec_t)) return kTRUE;
      if (typeid(T) == typeid(FloatVec_t)) return kTRUE;
      if (typeid(T) == typeid(std::vector<Double_t>)) return kTRUE;
      if (typeid(T) == typeid(std::vector<Bool_t>)) return kTRUE;
      if (typeid(T) == typeid(StringVec_t)) return kTRUE;
      return kFALSE;
   }

   void SetValue(art::TParameterStrings *param) {
      if (param->IsSet(GetName())) {
         param->GetValue(GetName(),fParameter);
         fIsValueSet = kTRUE;
      }
   }
   
   TString DefaultValue() { return ToString(fDefValue,fSize); }
   TString Value() { return ToString(fParameter,fSize); }
   template<class T1> const T1& GetRawValue() { return fParameter; }
   
   

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
   static TString ToString(Double_t prm, int) {
      TString str;
      str += prm;
      return str;
   }
   static TString ToString(TString prm, int) {
      TString str;
      str += prm;
      return str;
   }

protected:
   T &fParameter;
   T  fDefValue;
   
//   ClassDef(TParameter_t,1); // implementation of processor parameter
};
}
   

#endif // INCLUDE_GUARD_UUID_6D72126A_3ED4_434A_BA27_441B5C89DC2E
