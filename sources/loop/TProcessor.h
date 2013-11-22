/* $Id:$ */
/**
 * @file   TProcessor.h
 * @date   Created : Jul 10, 2013 17:10:49 JST
 *   Last Modified : Nov 22, 2013 10:34:28 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef ART_TPROCESSOR_H
#define ART_TPROCESSOR_H

#include <TEventCollection.h>
#include <TParameterStrings.h>
#include <TParameter.h>
#include <TLoop.h>

namespace art {
   class TProcessor;
}

namespace YAML {
   class Node;
}

void operator >> (const YAML::Node &node, art::TProcessor *&proc);
   
class art::TProcessor  : public TNamed {

public:
   typedef std::map<TString,TParameter*> ProcPrmMap_t;

   TProcessor();
   virtual ~TProcessor();

   // parant init should called
   void InitProc(TEventCollection *col) {
      fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
      Init(col);
   }
protected:
   virtual void Init (TEventCollection *) {;}
public:
   virtual void BeginOfRun() {;}
   virtual void EndOfRun() {;}
   virtual void Process() {;}
   virtual void PreLoop() {;}
   virtual void PostLoop() {;}

   virtual void SetStopLoop() { if (fCondition) (*fCondition)->Set(TLoop::kStopLoop); }
   virtual void SetStopEvent() { if (fCondition)  (*fCondition)->Set(TLoop::kStopEvent); }
   virtual void SetEndOfRun() { if (fCondition)  (*fCondition)->Set(TLoop::kEndOfRun); }
   
   virtual void PrintDescriptionYAML();


   // orverride the original function
   virtual void SetName(const char* name);
   // store the parameter values
   virtual void SetParameters(TParameterStrings *params);
   // update the parameter values
   virtual void UpdateParameters();

   virtual void Clear(Option_t* /* opt */);

protected:
   // cannot use set title directly
   virtual void SetTitle(const char* title = "") { TNamed::SetTitle(title); }
private:
   ProcPrmMap_t fParamMap;
      
   Bool_t fInitialized;

protected:
   Bool_t fOutputIsTransparent;    // output transparency
   TConditionBit **fCondition; // condition bit to control loop
   TParameterStrings *fParameters; // parameter strings
   
   // register processor parameter
   template<class T>
   void RegisterProcessorParameter(const char* name,
                                   const char* description,
                                  T& parameter,
                                  const T& defaultParam,
                                  int size = 0) {
      fParamMap[name] = new TParameter_t<T>(name,description,parameter,
                                               defaultParam,false,size);
   }                                               
   
   void RegisterInputCollection(const char* name,
                                const char* description,
                                StringVec_t& parameter,
                                const StringVec_t& defaultParam,
                                int size = 0) {
      RegisterProcessorParameter(name,description,parameter,
                                 defaultParam,size);
   }                                               

   void RegisterOutputCollection(const char* name,
                                 const char* description,
                                 TString& parameter,
                                 const TString& defaultParam,
                                 int size = 0) {
      RegisterProcessorParameter(name,description,parameter,
                                  defaultParam,size);
   }                                               

   template<class T>
   void RegisterOptionalParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam,
                                  int size = 0) {
      fParamMap[name] = new TParameter_t<T>(name,description,parameter,
                                               defaultParam,true,size);
   }                                               

};
#endif // end of #ifdef TPROCESSOR_H
