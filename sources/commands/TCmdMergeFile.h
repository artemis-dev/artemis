/* @file TCmdMergeFile.h
 * @brief merge files
 *
 * @date Create        : 2019-07-01 16:16:31 JST
 *       Last Modified : 2019-07-01 16:19:44 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_8658A594_39FD_4A91_8E41_3184AEB9287E
#define INCLUDE_GUARD_UUID_8658A594_39FD_4A91_8E41_3184AEB9287E

#include "TCatCmd.h"

namespace art {
   class TCmdMergeFile;
}


class art::TCmdMergeFile : public TCatCmd {
public:
   TCmdMergeFile();
   virtual ~TCmdMergeFile();


   virtual Long_t Cmd(vector<TString> args);

   static Long_t Run(const char* output, vector<TString> files);
   
   
   ClassDef(TCmdMergeFile,1)
}; // end of TCmdMergeFile

#endif // #ifndef INCLUDE_GUARD_UUID_8658A594_39FD_4A91_8E41_3184AEB9287E}
