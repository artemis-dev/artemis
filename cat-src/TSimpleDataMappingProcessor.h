/**
 * @file   TSimpleDataMappingProcessor.h
 * @brief  simple data mapper
 *
 * @date   Created       : 2014-05-19 00:22:01 JST
 *         Last Modified : 2018-02-04 13:08:26 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_95757E78_242F_4EBD_BFFA_626F22AB5D1D
#define INCLUDE_GUARD_UUID_95757E78_242F_4EBD_BFFA_626F22AB5D1D

#include <TProcessor.h>

namespace art {
  class TSimpleDataMappingProcessor;
  class TCategorizedData;
}

class TClonesArray;

class art::TSimpleDataMappingProcessor : public TProcessor {
 public:
  TSimpleDataMappingProcessor();
  virtual ~TSimpleDataMappingProcessor();

  virtual void Init(TEventCollection *col);
  virtual void Process();

 protected:
  TString            fInputColName;
  TString            fOutputColName;
   TCategorizedData **fCategorizedData; //!
   TClonesArray      *fOutputArray; //!

  Int_t fCatID;       // category id
  Int_t fDataTypeID;  // typeid for data

  Bool_t fIsSparse;

 private:
  TSimpleDataMappingProcessor(const TSimpleDataMappingProcessor&);
  TSimpleDataMappingProcessor& operator=(const TSimpleDataMappingProcessor&);

  ClassDef(TSimpleDataMappingProcessor,0) // simple data mapper
};

#endif // INCLUDE_GUARD_UUID_95757E78_242F_4EBD_BFFA_626F22AB5D1D
