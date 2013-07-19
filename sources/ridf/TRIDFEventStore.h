/* $Id:$ */
/**
 * @file   TRIDFEventStore.h
 * @date   Created : Jul 12, 2013 17:12:43 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRIDFEVENTSTORE_H
#define TRIDFEVENTSTORE_H

#include <TProcessor.h>

namespace art {
   class TRIDFEventStore;
   class TConditionBit;
   class TMapTable;
   class TSegmentedData;
   class TCategorizedData;
   class TDataSource;
}

class art::TRIDFEventStore  : public TProcessor {
   
public:
   static const Int_t kMaxBufSize = 1024*1024; // default size is 1 MB
   TRIDFEventStore();
   virtual ~TRIDFEventStore();
   
   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TConditionBit   **fCondition; //!
   struct RIDFData {
      TMapTable        *fMapTable; //!
      TSegmentedData   *fSegmentedData;
      TCategorizedData *fCategorizedData;
   } fRIDFData;
   TDataSource      *fDataSource;

   Bool_t fIsOnline;
   Int_t  fIsEOB;
   
   StringVec_t       fFileName;
   TString           fNameSegmented;
   TString           fNameCategorized;;
   TString           fMapConfigName;

   Long_t   fMaxEventNum;
   Long_t   fEventNum; //! local Event number

   Int_t    fMaxBufSize; // maximum buffer size
   Char_t  *fBuffer; // local buffer for the data read
   Int_t    fOffset; // read offset
   Int_t    fBlockSize;   // size of read block

   class RIDFHeader {
   private:
      unsigned long long int fHeader; // header
   public:
      Int_t NumOfWords() { return ((fHeader>>32) & 0x3fffff); }
      Int_t Size() { return NumOfWords()*sizeof(short); }
      Int_t ClassID() { return ((fHeader >> 54) & 0x3f); }
      Int_t Layer() { return ((fHeader >> 60) & 0x3); }
      Int_t Address() { return (fHeader & 0xffffffff); }
      unsigned long long int& operator=(unsigned long long int val) { return (fHeader = val); }
   } fHeader;  //! temporary data to analyze header

   // use function pointer array to decode classes instead of function object.
   // since the number of classes may not change so much and they are easy to be maintained.
   void (*fClassDecoder[64])(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata); //!

   // dummy decoder to skip the data
   static void ClassDecoderSkip(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // unknown class decoder to skip the data
   static void ClassDecoderUnknown(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // decode the event header
   static void ClassDecoder03(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // decode the segment
   static void ClassDecoder04(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // decode the comment header
   static void ClassDecoder05(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // decode the time stamp event header
   static void ClassDecoder06(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);

//   ClassDef(TRIDFEventStore,1);
   
};
#endif // end of #ifdef TRIDFEVENTSTORE_H
