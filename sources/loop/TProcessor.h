/* $Id:$ */
/**
 * @file   TProcessor.h
 * @date   Created : Jul 10, 2013 17:10:49 JST
 *   Last Modified : May 22, 2014 22:35:28 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TPROCESSOR_H
#define ART_TPROCESSOR_H

#include <TEventCollection.h>
#include <TParameterStrings.h>
#include <TProcessorParameter.h>
#include <TLoop.h>

namespace art {
   class TProcessor;
   namespace ErrMsgFmt {
      const char* const INVALID_INPUT_COLLECTION = "No such input collection %s";
      const char* const INPUT_CLASS_MISSMATCH = "Input class %s required while %s is found";
      const char* const INPUT_DATA_CLASS_MISSMATCH = "Input data class %s required while %s is found";
      const char* const NOT_EXIST_CLASS = "No such class %s";
      const char* const INVALID_IDLE_STATE = "State is not set correctly";
      const char* const NOT_EXIST_DATA_CLASS = "No such data class %s for TClonesArray";
      const char* const OUTPUT_ALREADY_EXIST = "Output collection '%s' already exist";
   }
}

namespace YAML {
   class Node;
}

void operator >> (const YAML::Node &node, art::TProcessor *&proc);
   
class art::TProcessor  : public TNamed {
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

   virtual void PrintDescriptionYAML();


   // orverride the original function
   virtual void SetName(const char* name);
   // store the parameter values
   virtual void SetParameters(TParameterStrings *params);
   // update the parameter values
   virtual void UpdateParameters();

   virtual void Clear(Option_t* /* opt */);

protected:
   // user defined initialization
   virtual void Init (TEventCollection *) {;}
   // cannot use set title directly
   virtual void SetTitle(const char* title = "") { TNamed::SetTitle(title); }

   class InputCollection {
   public:
      InputCollection(void* p,TString* name, TString className, TString dataClassName)
         : fP(p), fName(name),fClassName(className),fDataClassName(dataClassName) {;}
      void* fP; 
      TString *fName; 
      TString fClassName;
      TString fDataClassName;
   } ;
   class OutputCollection {
   public:
      OutputCollection(void* p,TString* name, TString className, TString dataClassName)
         : fP(p), fName(name),fClassName(className),fDataClassName(dataClassName) {;}
      void* fP;
      TString* fName;
      TString fClassName;
      TString fDataClassName;
   };
      
   // register processor parameter
   template<class T>
   void RegisterProcessorParameter(const char* name,
                                   const char* description,
                                   T& parameter,
                                   const T& defaultParam) {
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
         fInputs.push_back(InputCollection(p, &parameter,inputclass,dataclass));
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
         fOutputs.push_back(OutputCollection(p, &parameter,outputclass,dataclass));
      }
   }                                               

   template<class T>
   void RegisterOptionalParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam) {
      fParamMap[TString(name)] = new TParameter_t<T>(name,description,parameter,
                                               defaultParam,true);
   }
#if 0
   template<class T>
   void RegisterInputInfo(const char* name, const char* description,
                          TString &parameter, const TString& defaultParam,
                          void **p = NULL,
                          TString infoclass = "");

#endif
   
   // protected members
   
   Bool_t fOutputIsTransparent;    // output transparency
   TConditionBit **fCondition; // condition bit to control loop
   TParameterStrings *fParameters; // parameter strings
   Int_t  fVerboseLevel; // verbose level
   std::vector<InputCollection> fInputs;//!
   std::vector<OutputCollection> fOutputs;//!

private:
   // parameter map to hold the processor parameters
   ProcPrmMap_t fParamMap;
   State_t fState; // state of initialization
   TString fErrorMessage; // error message to be filled by user
   
};
#endif // end of #ifdef TPROCESSOR_H
