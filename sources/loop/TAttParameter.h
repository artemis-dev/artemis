/* @file TAttParameter.h
 * @brief attribute of parameter
 *
 * @date Create        : 2019-06-04 09:47:23 JST
 *       Last Modified : 2023-02-13 16:40:19 JST
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_695239A4_45D3_4F4B_B355_6B03257477BE
#define INCLUDE_GUARD_UUID_695239A4_45D3_4F4B_B355_6B03257477BE

#include "TString.h"
#include "TClass.h"
#include <typeinfo>
#include "TClonesArray.h"

namespace art {
   class TAttParameter;
   
}

class TClonesArray;

class art::TAttParameter {
public:
   TAttParameter();
   virtual ~TAttParameter();

   template <typename T> class Parameter;
   class ObjectIO;
   template <typename T, typename DataClass = TObject> class InputObject;
   template <typename T, typename DataClass = TObject> class OutputObject;
   template <typename T, typename DataClass = TObject> class InputData;
   template <typename T, typename DataClass = TObject> class OutputData;
   template <typename T, typename DataClass = TObject> class InputInfo;
   template <typename T, typename DataClass = TObject> class OutputInfo;

   template <typename T>
   class Parameter {
   public:
      Parameter() {}
      Parameter(const char* name, const char* title, T defaultValue)
         : fValue(defaultValue), fName(name), fTitle(title), fDefaultValue(defaultValue) {}
      virtual void SetContents(const char* name, const char* title, T defaultValue) {
         fName = name;
         fTitle = title;
         fDefaultValue = defaultValue;
      }
      virtual Parameter<T>* operator() (const char* name, const char* title, T defaultValue) {
         SetContents(name, title, defaultValue);
         return this;
      }
      virtual const char* GetName() const { return fName; }
      virtual const char* GetTitle() const { return fTitle; }
      virtual bool IsObject() const { return false; }
      virtual bool IsData() const { return false; }
      virtual bool IsInput() const { return true;}
      virtual bool IsInputData() const  { return IsData() && IsInput(); }
      virtual bool IsInputInfo() const  { return !IsData() && IsInput(); }
      virtual bool IsOutputData() const { return IsData() && !IsInput(); }
      virtual bool IsOutputInfo() const { return !IsData() && !IsInput(); }

      virtual T& Value() { return fValue; }
      virtual const T& Value() const { return fValue; }
      virtual const T& DefaultValue() const { return fDefaultValue; }
//   operator T&() { return fValue; }
      operator T() const { return fValue; }
      virtual T& operator=(T val) { fValue = val; return fValue; }
      T fValue;
      T fDefaultValue;
      TString fName;
      TString fTitle;
   };


   class VecIO: public Parameter<TString> {
     public:
     virtual void SetDoAuto(bool flag) { fDoAuto = flag; }
     virtual bool IsObject() const { return false; }
     virtual bool IsData() const = 0;
     virtual bool IsInput() const = 0;
     virtual void* Ptr() { return NULL; }
     virtual void* PtrRef() { return NULL; }
     virtual const char* ClassName() const { return ""; }

     Bool_t fDoAuto;
   };

   template <class T>
   class InputVec : public VecIO {
     public:
     using datatype = std::vector<T>;
      InputVec() : fData(NULL) {
         fDoAuto = kTRUE;
      }
      virtual bool IsInput() const { return true; }
      virtual bool IsData() const { return true; }
      virtual void SetData(datatype** data) { fData = data; }
      virtual const datatype* Data() const { return  (fData ? *fData : NULL) ; }
      virtual datatype* Data() { return  (fData ? *fData : NULL) ; }
      virtual void* Ptr() { return fData; }
      virtual void* PtrRef() { return &fData; }
      virtual inline const datatype* operator->() const { return Data(); }
      virtual inline datatype* operator->() { return Data(); }
      datatype** fData;
   };

   template <class T>
   class OutputVec : public VecIO {
     public:
     using datatype = std::vector<T>;
      OutputVec() : fData(NULL) {
         fDoAuto = kTRUE;
      }
      virtual bool IsInput() const { return false; }
      virtual bool IsData() const { return true; }
      virtual void SetData(datatype* data) { fData = data; }
      virtual const datatype* Data() const { return  fData; }
      virtual datatype* Data() { return  fData; }
      virtual void* Ptr() { return fData; }
      virtual void* PtrRef() { return &fData; }
      virtual inline const datatype* operator->() const {return fData; }
      virtual inline datatype* operator->() {return fData; }
      datatype* fData;
   };
   
   class ObjectIO : public Parameter<TString> {
   public:
      Bool_t fDoAuto;
      TString fDataClassName;
      virtual void SetDoAuto(bool flag) { fDoAuto = flag; }
      virtual bool IsObject() const { return true; }
      virtual bool IsInput() const = 0;
      virtual bool IsData() const = 0;
      virtual void* Ptr() { return NULL; }
      virtual void* PtrRef() { return NULL; }
      virtual const char* ClassName() const { return ""; }
   };

   template <class T, class dataClass>
   class InputObject : public ObjectIO {
   public:
      InputObject() : fData(NULL) {
         fDoAuto = kTRUE;
         if (typeid(T) == typeid(TClonesArray)
             && typeid(dataClass) != typeid(TObject)) {
            fDataClassName = dataClass::Class_Name();
         }
      }
      virtual bool IsInput() const { return true; }
      virtual bool IsInitialized() const { return (fData != NULL); }
      virtual void SetData(T** data) { fData = data; }
      virtual const T* Data() const { return  (fData ? *fData : NULL) ; }
      virtual T* Data() { return  (fData ? *fData : NULL) ; }
      virtual void* Ptr() { return fData; }
      virtual void* PtrRef() { return &fData; }
      virtual const char* ClassName() const { return T::Class_Name(); }
      virtual inline const T* operator->() const { return Data(); }
      virtual inline T* operator->() { return Data(); }
      T** fData;
   };

   template <class T, class dataClass>
   class OutputObject : public ObjectIO {
   public:
      OutputObject() : fData(NULL) {
         fDoAuto = kTRUE;
         if (typeid(T) == typeid(TClonesArray)
             && typeid(dataClass) != typeid(TObject)) {
            fDataClassName = dataClass::Class_Name();
         }
      }
      virtual bool IsInput() const { return false; }
      virtual void SetData(T* data) { fData = data; }
      virtual bool IsInitialized() const { return (fData != NULL); }
      virtual const T* Data() const { return  fData; }
      virtual T* Data() { return  fData; }
      virtual void* Ptr() { return fData; }
      virtual void* PtrRef() { return &fData; }
      virtual const char* ClassName() const { return T::Class_Name();  }
      virtual inline const T* operator->() const {return fData; }
      virtual inline T* operator->() {return fData; }
      
      T* fData;
   };


   template <class T, class dataClass>
   class InputData : public InputObject<T,dataClass> {
   public:
      virtual bool IsData() const { return true; }
   };
   template <class T, class dataClass>
   class OutputData : public OutputObject<T,dataClass> {
   public:
      virtual bool IsData() const { return true; }
   };
   template <class T, class dataClass>
   class InputInfo : public InputObject<T,dataClass> {
   public:
      virtual bool IsData() const { return false; }
   };
   template <class T, class dataClass>
   class OutputInfo : public OutputObject<T,dataClass> {
   public:
      virtual bool IsData() const { return false; }
   };

//   ClassDef(TAttParameter,1)
};





#endif // #ifndef INCLUDE_GUARD_UUID_695239A4_45D3_4F4B_B355_6B03257477BE}
