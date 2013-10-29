/* $Id:$ */
/**
 * @file   TRIDFEventStore.h
 * @date   Created : Jul 12, 2013 17:12:43 JST
 *   Last Modified : Oct 29, 2013 16:33:24 JST
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

   /*----------------------------------------
    * Structure for header 
    *
    */
   class RIDFHeader {
   private:
      unsigned long long int fHeader; // header
   public:
      Int_t NumOfWords() { return ((fHeader) & 0x3fffff); }
      Int_t Size() { return NumOfWords()*sizeof(short); }
      Int_t ClassID() { return ((fHeader >> 22) & 0x3f); }
      Int_t Layer() { return ((fHeader >> 28) & 0x3); }
      Int_t Address() { return ((fHeader>>32) & 0xffffffff); }
      unsigned long long int& operator=(unsigned long long int val) { return (fHeader = val); }
      void Print() {
         printf("data    = 0x%8x\n",fHeader);
         printf("Size    = %d\n",Size());
         printf("ClassID = %d\n",ClassID());
         printf("Layer   = %d\n",Layer());
         printf("Address = %d\n",Address());
      }
   } fHeader;  //! temporary data to analyze header


   /*----------------------------------------
    * Structure for run comment 
    *
    */
   struct RIDFCommentRunInfo {
      char fRunName[100];
      char fRunNumber[100];
      char fStartTime[20];
      char fStopTime[20];
      char fDate[20];
      char fReserved[40];
      char fHeader[100];
      char fEnder[100];
      void Print() {
         printf(" RUN   : %s%s\n",fRunName,fRunNumber);
         printf(" Start : %s %s\n",fDate, fStartTime);
         printf(" Stop  : %s  %s\n",fDate, fStopTime);
         printf(" Header: %s\n",fHeader);
         printf(" Ender : %s\n",fEnder);
      }
   };
      
   /*----------------------------------------
    * Structure for event segment header 
    *
    */
   class SegID {
   public:
      Int_t Module() { return (fSegID & 0xff); }
      Int_t Detector() { return ((fSegID >> 8) & 0x3f); }
      Int_t FP() { return ((fSegID >> 14) & 0x3f); }
      Int_t Device() { return ((fSegID>>20) & 0x3f); }
      // return segid but module id is removed
      UInt_t Get() { return (fSegID & 0xffffff00); }
   private:
      unsigned int fSegID;
   };
      


//   ClassDef(TRIDFEventStore,1);
   
};
#endif // end of #ifdef TRIDFEVENTSTORE_H
