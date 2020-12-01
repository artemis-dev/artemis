/* $Id:$ */
/**
 * @file   TMappingProcessor.h
 * @date   Created : Nov 22, 2013 17:22:13 JST
 *   Last Modified : 2020-11-27 00:14:13 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TMAPPINGPROCESSOR_H
#define TMAPPINGPROCESSOR_H

#include "TProcessor.h"
#include <vector>

namespace art {
   class TMappingProcessor;
   class TCategorizedData;
   class TSegmentedData;
   class TMapTable;
}

class art::TMappingProcessor  : public TProcessor {

public:
   typedef enum { RIDF, RDF } FormatType_t; // type of format
   

   TMappingProcessor();
   ~TMappingProcessor();

   virtual void Process();

   static void SetFormatType(FormatType_t type) { fgFormatType = type; }
   
protected:
   virtual void Init(TEventCollection *col);

   static FormatType_t fgFormatType;
   
   StringVec_t fInputColName; // Name of input collections
   TString     fNameCategorized; // name of output collection
   TString     fMapConfigName; // name of mapper configuration file
   TCategorizedData  *fCategorizedData; //! pointer to categorized data
   TSegmentedData   **fSegmentedData; //! pointer to segmented data
   std::vector<TSegmentedData**> fSegmentedDataArray; //!
   TMapTable         *fMapTable; //! pointer to map table

   ClassDef(TMappingProcessor,1); // mapping processor from TSegmentedData to TCategorizedData
};
#endif // end of #ifdef TMAPPINGPROCESSOR_H
