/* $Id:$ */
/**
 * @file   TProcessor.h
 * @date   Created : Jul 10, 2013 17:10:49 JST
 *   Last Modified : 2019-04-26 10:38:24 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TPROCESSOR_H
#define ART_TPROCESSOR_H

#include "TEventCollection.h"
#include "TParameterStrings.h"
#include "TProcessorParameter.h"
#include "TLoop.h"
#include <iostream>
#include "TClass.h"
#include "TClonesArray.h"
#include "TAttParameter.h"

namespace art {
   class TProcessor;
   class IProcessorHelper;
   namespace ErrMsgFmt {
      const char* const INVALID_INPUT_COLLECTION = "No such input collection %s";
      const char* const INPUT_CLASS_MISSMATCH = "Input class %s required while %s is found";
      const char* const INPUT_DATA_CLASS_MISSMATCH = "Input data class %s required while %s is found";
      const char* const NOT_EXIST_CLASS = "No such class %s";
      const char* const INVALID_IDLE_STATE = "State is not set correctly";
      const char* const NOT_EXIST_DATA_CLASS = "No such data class %s for TClonesArray";
      const char* const OUTPUT_ALREADY_EXIST = "Output collection '%s' already exist";
   }
   R__EXTERN TList *gProcessors;
}

class TClonesArray;



namespace YAML {
   class Node;
}

void operator >> (const YAML::Node &node, art::TProcessor *&proc);
   
class art::TProcessor  : public TNamed, public TAttParameter {
public:
   typedef std::map<TString,TProcessorParameter*> ProcPrmMap_t;
   typedef enum { ERROR=-1, INIT, READY} State_t;

   TProcessor();
   virtual ~TProcessor();

   // common initialization 
   void InitProc(TEventCollection *col);

   // user defined member functions to process data
   virtual void BeginOfRun() {;}
   virtual void EndOfRun() {;}
   virtual void Process() {;}
   virtual void PreLoop() {;}
   virtual void PostLoop() {;}
   virtual void Terminate() {;}

   virtual void SetStopLoop() { if (fCondition) (*fCondition)->Set(TLoop::kStopLoop); }
   virtual void SetStopEvent() { if (fCondition)  (*fCondition)->Set(TLoop::kStopEvent); }
   virtual void SetEndOfRun() { if (fCondition)  (*fCondition)->Set(TLoop::kEndOfRun); }

   // get initialization state
   virtual State_t GetState() const { return fState; }

   // check if the state is initial state
   virtual Bool_t IsInit() const { return (fState == INIT); }
   // itinialization is error
   virtual Bool_t IsError() const { return (fState == ERROR); }
   // initialization is succeeded
   virtual Bool_t IsReady() const { return (fState == READY); }
   

   // set initialization state
   virtual void SetState(State_t state) { fState = state; }
   // set the state to be ready for process
   virtual void SetStateReady() { SetState(READY); }
   // set the state to be error
   virtual void SetStateError(const char* message) {
      SetState(ERROR);
      SetErrorMessage(message);
   }
   // Set the state to be initial
   virtual void SetStateInit() { SetState(INIT); }
   // get error message
   virtual const char* GetErrorMessage() { return fErrorMessage; }
   // set error message
   virtual void SetErrorMessage(const char* message) { fErrorMessage = message; }

   virtual void PrintDescriptionYAML(std::ostream& ros = std::cout) const;

   virtual Int_t GetVerboseLevel() const { return fVerboseLevel; } 


   // orverride the original function
   virtual void SetName(const char* name);
   // store the parameter values
   virtual void SetParameters(TParameterStrings *params);
   // update the parameter values
   virtual void UpdateParameters();

   virtual void Clear(Option_t* /* opt */);

   static void ListProcessors();

   // user defined initialization
   virtual void Init (TEventCollection *) {;}
   // cannot use set title directly
   virtual void SetTitle(const char* title = "") { TNamed::SetTitle(title); }

   class IOCollection {
   public:
      IOCollection(void* p,TString* name, TString className, TString dataClassName,TString prmname = "")
         : fP(p), fName(name),fClassName(className),fDataClassName(dataClassName),fPrmName(prmname),fCapacity(1),fIsObject(kTRUE) {;}
      IOCollection(void* p,TString* name, TString type, Int_t capacity, TString *length) 
         : fP(p), fName(name),fClassName(type),fDataClassName(""),fPrmName(""),fCapacity(capacity),fIsObject(kFALSE),fLength(length) {;}
      void* fP; 
      TString *fName; 
      TString fClassName;
      TString fDataClassName;
      TString fPrmName;
      Int_t fCapacity;
      Bool_t fIsObject;
      TString *fLength;
   } ;
   
   
   
   
   Bool_t ParameterExists(const char* name) {
      if (fParamMap.find(TString(name)) != fParamMap.end()) {
         return true;
      }
      return false;
   }
   
   void RemoveParameter(const char* name) {
      fParamMap.erase(fParamMap.find(TString(name)));
   }
   void RemoveIO(std::vector<art::TProcessor::IOCollection>& iocol, const char* name) {
      RemoveParameter(name);
      for (std::vector<IOCollection>::iterator it = fOutputs.begin(), itend = fOutputs.end();
           it != itend; ++it) {
         if ((*it).fPrmName == TString(name)) {
            fOutputs.erase(it);
            break;
         }
      }
   }
      
   // register processor parameter
   template<class T>
   void RegisterProcessorParameter(const char* name,
                                   const char* description,
                                   T& parameter,
                                   const T& defaultParam) {
      if (ParameterExists(name)) {
         Warning("RegisterProcessorParameter",
                 "Parameter '%s' exists already. Use OverrideProcessorParameter instead.",
                 name);
      }
      fParamMap[TString(name)] = new TParameter_t<T>(name,description,parameter,
                                               defaultParam,false);
   }                                               
   
   void RegisterInputCollection(const char* name,
                                const char* description,
                                StringVec_t& parameter,
                                const StringVec_t& defaultParam) {
      RegisterProcessorParameter(name,description,parameter,
                                 defaultParam);
   }                                               
   
   void RegisterInputCollection(const char* name,
                                const char* description,
                                TString& parameter,
                                const TString& defaultParam,
                                void* p = NULL,
                                TString inputclass = "",
                                TString dataclass = "") {
      RegisterProcessorParameter(name,description,parameter,
                                 defaultParam);
      if (p) {
         fInputs.push_back(IOCollection(p, &parameter,inputclass,dataclass,name));
      }
   }
   
   
   
   void RegisterOutputCollection(const char* name,
                                 const char* description,
                                 TString& parameter,
                                 const TString& defaultParam,
                                 void * p = NULL,
                                 TString outputclass = "",
                                 TString dataclass = "") {
      RegisterProcessorParameter(name,description,parameter,
                                 defaultParam);
      if (p) {
         fOutputs.push_back(IOCollection(p, &parameter,outputclass,dataclass,name));
      }
   }
   
   
   void OverrideOutputCollection(const char* name,
                                 const char* description,
                                 TString& parameter,
                                 const TString& defaultParam,
                                 void * p = NULL,
                                 TString outputclass = "",
                                 TString dataclass = "") {
      RemoveIO(fOutputs,name);
      RegisterOutputCollection(name,description,parameter,defaultParam,p,outputclass,dataclass);
   }
   
   
   
   // register output collection for primitive class
   void RegisterOutputCollection(const char* name,
                                 const char* description,
                                 TString& parameter,
                                 const TString& defaultParam,
                                 TString type,
                                 void* p,
                                 Int_t capacity = 1,
                                 TString *length = NULL) {
      RegisterProcessorParameter(name,description,parameter,
                                 defaultParam);
      if (p) {
         fOutputs.push_back(IOCollection(p, &parameter,type,capacity,length));
      }
   }
   
   void OverrideOutputCollection(const char* name,
                                 const char* description,
                                 TString& parameter,
                                 const TString& defaultParam,
                                 TString type,
                                 void* p,
                                 Int_t capacity = 1,
                                 TString *length = NULL) {
      RemoveIO(fOutputs,name);
      RegisterOutputCollection(name,description,parameter,defaultParam,type,p,capacity,length);
   }
   
   template<class T>
   void RegisterOptionalParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam) {
      fParamMap[TString(name)] = new TParameter_t<T>(name,description,parameter,
                                                     defaultParam,true);
   }
   
   template<class T>
   void OverrideOptionalParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam) {
      RemoveParameter(name);
      RegisterOptionalParameter(name,description,parameter,defaultParam);
   }
   
   
   void RegisterInputInfo(const char* name, const char* description,
                          TString &parameter, const TString& defaultParam,
                          void *p = NULL,
                          TString infoclass = "", TString dataclass = "") {
      RegisterProcessorParameter(name,description,parameter,defaultParam);
      if (p) {
         fInputInfo.push_back(IOCollection(p, &parameter,infoclass,dataclass,name));
      }
   }
   void RegisterOutputInfo(const char* name, const char* description,
                           TString &parameter, const TString& defaultParam) 
      {
         
      RegisterProcessorParameter(name,description,parameter,defaultParam);
   }
   void RegisterOptionalInputInfo(const char* name, const char* description,
                                  TString &parameter, const TString& defaultParam,
                                  void *p = NULL,
                                  TString infoclass = "", TString dataclass = "") {
      RegisterOptionalParameter(name,description,parameter,defaultParam);
      if (p) {
         fInputInfo.push_back(IOCollection(p, &parameter,infoclass,dataclass,name));
      }
   }
   
   template <class T>
   void RegisterProcessorParameter(Parameter<T>* param) {
      RegisterProcessorParameter(param->fName, param->fTitle, param->fValue, param->fDefaultValue);
   }
   

   void RegisterInputCollection(ObjectIO* input) {
      if (input->fDoAuto) {
         RegisterInputCollection(input->fName, input->fTitle, input->fValue, input->fDefaultValue,
                                 input->PtrRef(), input->ClassName(), input->fDataClassName);
      } else {
         RegisterInputCollection(input->fName, input->fTitle, input->fValue, input->fDefaultValue);
      }
   }
   void RegisterOutputCollection(ObjectIO* output) {
      if (output->fDoAuto) {
         RegisterOutputCollection(output->fName, output->fTitle, output->fValue, output->fDefaultValue,
                                  output->PtrRef(), output->ClassName(), output->fDataClassName);
      } else {
         RegisterOutputCollection(output->fName, output->fTitle, output->fValue, output->fDefaultValue);
      }
   }
   void RegisterInputInfo(ObjectIO* input) {
      if (input->fDoAuto) {
         RegisterInputInfo(input->fName, input->fTitle, input->fValue, input->fDefaultValue,
                           input->PtrRef(), input->ClassName(), input->fDataClassName);
      } else {
         RegisterInputInfo(input->fName, input->fTitle, input->fValue, input->fDefaultValue);
      }
   }
   void RegisterOutputInfo(ObjectIO* output) {
      RegisterOutputInfo(output->fName, output->fTitle, output->fValue, output->fDefaultValue);
   }

template <class T> 
   void Register(Parameter<T>* prm) {
      if (prm->IsInputData()) {
         Info("Register","InputData");
         RegisterInputCollection(dynamic_cast<ObjectIO*>(prm));
      } else if (prm->IsInputInfo()) {
         Info("Register","InputInfo");
         RegisterInputInfo(dynamic_cast<ObjectIO*>(prm));
      } else if (prm->IsOutputData()) {
         Info("Register","OutputData");
         RegisterOutputCollection(dynamic_cast<ObjectIO*>(prm));
      } else if (prm->IsOutputInfo()) {
         Info("Register","OutputInfo");
         RegisterOutputInfo(dynamic_cast<ObjectIO*>(prm));
      } else {
         RegisterProcessorParameter(prm);
      }
   }


   void RegisterHelper(IProcessorHelper *helper);
   
   // protected members
   
protected:
   Bool_t fOutputIsTransparent;    // output transparency
   TConditionBit **fCondition; //! condition bit to control loop
   TParameterStrings *fParameters; //! parameter strings
   Int_t  fVerboseLevel; // verbose level
   std::vector<IOCollection> fInputs;//!
   std::vector<IOCollection> fOutputs;//!
   std::vector<IOCollection> fInputInfo; //!
   std::vector<IProcessorHelper*> fHelpers; //!

private:
   // parameter map to hold the processor parameters
   ProcPrmMap_t fParamMap; //
   State_t fState; // state of initialization
   TString fErrorMessage; //! error message to be filled by user

   ClassDef(TProcessor,1);
   
};
#endif // end of #ifdef TPROCESSOR_H
