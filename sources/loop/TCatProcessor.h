/* $Id:$ */
/**
 * @file   TCatProcessor.h
 * @date   Created : Feb 19, 2012 10:19:49 JST
 *   Last Modified : May 18, 2012 16:02:09 JST
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

class TCatProcessor  : public TNamed {

public:
   TCatProcessor();
   ~TCatProcessor();

   virtual void Init(TCatEventCollection* /* collection */) { }
   virtual void BeginOfRun() { }
   virtual void EndOfRun() { }
   virtual void Process() { }
   virtual void Check() { }
   virtual void End() { }

   virtual void SetName(const char* name);

   virtual void SetWidget(TCatLoopWidget *widget) { fWidget = widget; }
   virtual void Info(const char *info) { fWidget->Info(info); }

   virtual void Clear(Option_t* /* opt */);

//   virtual void SetParameters(TCatStringParameters prm);
//   virtual void UpdateParameters();

protected:
   // should be called if the processor is initilized
   virtual void Initilized();

   // cannot use set title directory
   virtual void SetTitle(const char* title = "") { TNamed::SetTitle(title); }
   TCatParameterStrings fParameters;
private:
   std::map<TString,TCatParameter*> fParamMap;
      
   TCatLoopWidget *fWidget;

   Bool_t fInitialized;


   // register processor parameter
   template<class T>
   void RegisterProcessorParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam,
                                  int size) {
      fParamMap[name] = new TCatParameter_t<T>(name,description,parameter,
                                               defaultParam,false,size);
   }                                               

   template<class T>
   void RegisterOptionalParameter(const TString& name,
                                  const TString& description,
                                  T& parameter,
                                  const T& defaultParam,
                                  int size) {
      fParamMap[name] = new TCatParameter_t<T>(name,description,parameter,
                                               defaultParam,true,size);
   }                                               

   ClassDef(TCatProcessor,1);
};
#endif // end of #ifdef TCATPROCESSOR_H
