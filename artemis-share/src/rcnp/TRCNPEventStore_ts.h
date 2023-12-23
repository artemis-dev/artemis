/* $Id:$ */
/**
 * @file   TRCNPEventStore_ts.h
 * @date   Created : Jul 12, 2013 17:12:43 JST
 *   Last Modified : 2023-10-20 21:06:17 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRCNPEVENTSTORE_ts_H
#define TRCNPEVENTSTORE_ts_H

#include <TProcessor.h>
#include <IEventStore.h>
#include <arpa/inet.h>

namespace art {
   class TRCNPEventStore_ts;
   class TConditionBit;
   class TMapTable;
   class TSegmentedData;
   class TCategorizedData;
   class TDataSource;
   class TRCNPEventHeader;
   class TModuleDecoderFactory;
}

class art::TRCNPEventStore_ts  : public TProcessor, public IEventStore {
   
public:
   static const Int_t kMaxBufSize = 1024*1024*50; // default size is 1 MB
   TRCNPEventStore_ts();
   virtual ~TRCNPEventStore_ts();
   
   virtual void Init(TEventCollection *col);
   virtual void Process();
protected:
   Bool_t Open();
   Bool_t GetNextBlock();
   Bool_t GetNextEvent();

protected:
   struct RIDFData {
      TMapTable        *fMapTable; //!
      TSegmentedData   *fSegmentedData;
      TCategorizedData *fCategorizedData;
      TList   *fRunHeaders;
      TRCNPEventHeader *fEventHeader;
      Int_t        *fVerboseLevel;
      TModuleDecoderFactory *fDecoderFactory;
   } fRIDFData; //!
   TDataSource      *fDataSource; //!

   Bool_t fIsOnline;
   Int_t  fIsEOB;
   
   StringVec_t       fFileName;
   TString           fNameSegmented;
   TString           fNameCategorized;;
   TString           fMapConfigName;
   TString           fNameRunHeaders;
   TString           fNameEventHeader;
   TString           fSourceName;
   TString           fSearchPath;

   Long_t   fMaxEventNum;
   Long_t   fEventNum; //! local Event number
   Long_t   fEventNumTotal; //! total event number

   Int_t    fMaxBufSize; //! maximum buffer size
   Char_t  *fBuffer; //! local buffer for the data read
   Int_t    fOffset; //! read offset
   Int_t    fBlockSize;   //! size of read block
	 Int_t evtsize;
	 Int_t blksize;
	 Int_t nblock;
	 Int_t evtnum;

   // for online mode
   static const Int_t kSHM_BUFF_SIZE = 0x80000; // 256 kByte
   static const Int_t kSHMID_BASE    = 561000;
   static const Int_t kSEMKEY_BASE   = 561001;
   Int_t fSHMID;
   Int_t fBlockNumber;

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
   static void ClassDecoder04_2(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata, int sizee, int module);
   static void ClassDecoder05(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);
   // decode the time stamp event header
   static void ClassDecoder06(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata);

   /*----------------------------------------
    * Structure for header 
    *
    */
	 /*
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
         printf("data    = 0x%8Lx\n",fHeader);
         printf("Size    = %d\n",Size());
         printf("ClassID = %d\n",ClassID());
         printf("Layer   = %d\n",Layer());
         printf("Address = %d\n",Address());
      }
   } fHeader;  //! temporary data to analyze header
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
			//Int_t hedder(){ return ((fHeader)& 0xffff);}
			Int_t hedder(){ return ((fHeader>>8&0x00ff|fHeader<<8)& 0xffff);}
			Int_t getnum(){return ((fHeader >> 8 & 0x00ff)+(fHeader & 0x00ff)*0x0100);}//読み込みのときに逆になったbinaryを元に戻す
//			Int_t get_module_header(){return (fHeader&0x00ff);}
//			Int_t get_module_size(){return ((fHeader>>8)&0x00ff);}
//			Int_t get_module_header(){return ((fHeader>>4)&0x000f);}
			Int_t get_module_header(){return ((fHeader>>4)&0x000f);}
			Int_t get_module_size(){return ((fHeader>>8)&0x00ff)+((fHeader<<8)&0x0f00);}
      unsigned long long int& operator=(unsigned long long int val) { return (fHeader = val); }
      void Print() {
         printf("data    = 0x%8Lx\n",fHeader);
         printf("Size    = %d\n",Size());
         printf("ClassID = %d\n",ClassID());
         printf("Layer   = %d\n",Layer());
         printf("Address = %d\n",Address());
      }
   } fHeader;  //! temporary data to analyze header

   //////////////////////////////////////////////////////////////////////
   // bld1_header reader
   //////////////////////////////////////////////////////////////////////
   class bld1_header {
   public:
      int fId;
      int fSeq_num;
      int fBsize;
      int fHsize;
      int fPrev_pos1;
      int fPrev_pos2;
      int fNext_pos1;
      int fNext_pos2;
      static int Size() { return 4*8; }
      static int Magic() { return 0x424c4431; }
      void ReadFrom(int *buf) {
         fId = ntohl(buf[0]);
         fSeq_num = ntohl(buf[1]);
         fBsize = ntohl(buf[2]);
         fHsize = ntohl(buf[3]);
         fPrev_pos1 = ntohl(buf[4]);
         fPrev_pos2 = ntohl(buf[5]);
         fNext_pos1 = ntohl(buf[6]);
         fNext_pos2 = ntohl(buf[7]);
      }
      void Print() {
         printf("id = %x\n",fId);
         printf("seg_num = %d\n",fSeq_num);
         printf("bsize = %d\n",fBsize);
         printf("hsize = %d\n",fHsize);
         printf("prev1 = %d\n",fPrev_pos1);
         printf("prev2 = %d\n",fPrev_pos2);
         printf("next1 = %d\n",fNext_pos1);
         printf("next2 = %d\n",fNext_pos2);
      }
      
   } fBLD1Header;


   //////////////////////////////////////////////////////////////////////
   // blkheader
   //////////////////////////////////////////////////////////////////////
   class blkheader {
   public:
      unsigned short fValues[8];
      static int Size() { return 2*8; }
      uint16_t HeaderID()    const { return fValues[0]; }
      uint16_t HeaderSize()  const { return fValues[1]; }
      uint16_t BlockID()     const { return fValues[2]; }
      uint16_t BlockSize()   const { return fValues[3]; }
      uint16_t BlockNumber() const { return fValues[4]; }
      uint16_t NumEvents()   const { return fValues[5]; }
      uint16_t BlockSizeL()  const { return fValues[6]; }
      uint16_t BlockSizeU()  const { return fValues[7]; }
      
      void ReadFrom(short* buf) {
         for (int i = 0; i < 8; ++i) {
            fValues[i] = ntohs(buf[i]);
         }
      }

      void Print() {
         printf("HeaderID    = %x\n",HeaderID()    );
         printf("HeaderSize  = %u\n",HeaderSize()  );
         printf("BlockID     = %u\n",BlockID()     );
         printf("BlockSize   = %u\n",BlockSize()   );
         printf("BlockNumber = %u\n",BlockNumber() );
         printf("NumEvents   = %u\n",NumEvents()   );
         printf("BlockSizeL  = %u\n",BlockSizeL()  );
         printf("BlockSizeU  = %u\n",BlockSizeU()  );

      }
   } fBlkHeader;
      

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
			void SetSegID(int segidc){fSegID=segidc;}
   private:
      unsigned int fSegID;
   };
      
   ClassDef(TRCNPEventStore_ts,1) // Event store for ridf format files
};
#endif // end of #ifdef TRIDFEVENTSTORE_H
