/* $Id:$ */
/**
 * @file   TStreamingDataFileSinkComment
 * @date   Created : 2023-02-10 19:24:47 JST
 *   Last Modified : 2023-02-10 19:50:39 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *
 *    Copyright (C)2023
 */

#ifndef TSTREAMINGDATAFILESINKCOMMENT_H
#define TSTREAMINGDATAFILESINKCOMMENT_H

#include <stdint.h>
namespace art
{
   class TStreamingDataFileSinkComment;
}

constexpr uint64_t Magic{*(uint64_t *)(char *)"@FS-HEAD"};

class art::TStreamingDataFileSinkComment : public TNamed
{

public:
   TStreamingDataFileSinkComment();
   virtual ~TStreamingDataFileSinkComment();

   virtual Int_t ReadFrom(char *buf);

protected:
   static const uint64_t fMagic{Magic};
   uint64_t fSourceType; // source type before sink
   uint64_t fLength;     // size of comment
   uint64_t fStartTime;
   uint64_t fEndTime;
}

#endif // TSTREAMINGDATAFILESINKCOMMENT
