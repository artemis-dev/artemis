/* $Id:$ */
/**
 * @file   TCatProcessor.h
 * @date   Created : Feb 19, 2012 10:19:49 JST
 *   Last Modified : May 21, 2012 11:23:43 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPROCESSOR_H
#define TCATPROCESSOR_H


#include <TNamed.h>

#include <TCatLoopWidget.h>
#include <TCatEventCollection.h>

#include <TCatParameterStrings.h>
#include <TCatParameter.h>

#include <map>
#include <iostream>



class TCatProcessor  : public TNamed {
public:
   typedef std::map<TString,TCatParameter*> CatProcPrmMap_t;
public:
   TCatProcessor();
   virtual ~TCatProcessor();

   virtual void Init(TCatEventCollection* /* collection */) { }
   virtual void BeginOfRun() { }
   virtual void EndOfRun() { }
   virtual void Process() { }
   virtual void Check() { }
   virtual void End() { }

   virtual void Info(const char *info, const char* msgfmt = 0, ...) const { fWidget->Info(info); }


   virtual void PrintDescriptionYAML();


   // orverride the original function
   virtual void SetName(const char* name);
   // store the parameter values
   virtual void SetParameters(TCatParameterStrings *params);
   // set widget (should be obsoluted in the future)
   virtual void SetWidget(TCatLoopWidget *widget) { fWidget = widget; }
   // update the parameter values
   virtual void UpdateParameters();

   virtual void Clear(Option_t* /* opt */);

protected:
   // should be called if the processor is initilized
   virtual void Initilized();

   // cannot use set title directory
   virtual void SetTitle(const char* title = "") { TNamed::SetTitle(title); }
   TCatParameterStrings *fParameters;
private:
   CatProcPrmMap_t fParamMap;
      
   TCatLoopWidget *fWidget;

   Bool_t fInitialized;

protected:
   // register processor parameter
   template<class T>
   void RegisterProcessorParameter(const char* name,
                                   const char* description,
                                  T& parameter,
                                  const T& defaultParam,
                                  int size = 0) {
      fParamMap[name] = new TCatParameter_t<T>(name,description,parameter,
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
      fParamMap[name] = new TCatParameter_t<T>(name,description,parameter,
                                               defaultParam,true,size);
   }                                               

   ClassDef(TCatProcessor,1);
};
#endif // end of #ifdef TCATPROCESSOR_H
