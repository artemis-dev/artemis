/**
 * @file   TNyokiChargeCorrectionProcessor.h
 * @brief  processor for nyoki charge correction
 *
 * @date   Created       : 2015-04-16 06:35:58 JST
 *         Last Modified : 2018-02-04 12:05:33 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_59A26E72_C7E9_4660_8ECB_DFD0CCA03718
#define INCLUDE_GUARD_UUID_59A26E72_C7E9_4660_8ECB_DFD0CCA03718

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TNyokiChargeCorrectionProcessor;
   }
   class TMonotoneTableConverter;
   class TSplineConverter;
}

class TSpline3;
class TClonesArray;

class art::sh04::TNyokiChargeCorrectionProcessor : public TProcessor {
public:
   TNyokiChargeCorrectionProcessor();
   virtual ~TNyokiChargeCorrectionProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   typedef TMonotoneTableConverter    Converter_t;
   typedef std::vector<Converter_t*>  ConverterVec_t;
   typedef TSplineConverter           YConverter_t;
   typedef std::vector<YConverter_t*> YConverterVec_t;

   static const Int_t kNumNyoki = 14;

   /* input */
   TString        fInputCollectionName;
   TClonesArray **fInputCollection; //!
   TString        fTrackCollectionName;
   TClonesArray **fTrackCollection; //!

   /* output */
   TString        fOutputCollectionName;
   TClonesArray  *fOutputCollection; //!

   /* parameter */
   FloatVec_t      fPedestal;
   StringVec_t     fTableFiles;
   ConverterVec_t  fTable;
   StringVec_t     fYTableFiles;
   YConverterVec_t fYTable;

private:
   TNyokiChargeCorrectionProcessor(const TNyokiChargeCorrectionProcessor&);
   TNyokiChargeCorrectionProcessor& operator=(const TNyokiChargeCorrectionProcessor&);

   ClassDef(TNyokiChargeCorrectionProcessor,0) // processor for nyoki charge correction
};

#endif // INCLUDE_GUARD_UUID_59A26E72_C7E9_4660_8ECB_DFD0CCA03718
