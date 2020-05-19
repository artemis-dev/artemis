/* @file TCmdMergeFile.cc
 * @brief description
 *
 * @date Create        : 2019-07-01 16:18:03 JST
 *       Last Modified : 2019-07-01 17:14:38 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCmdMergeFile.h"
#include "TFileMerger.h"
#include "TSystem.h"
#include "TObjArray.h"
#include "TObjString.h"

ClassImp(art::TCmdMergeFile)

using art::TCmdMergeFile;

TCmdMergeFile::TCmdMergeFile()
{
   SetName("mergefile");
   SetTitle("merge histogram files");
}

TCmdMergeFile::~TCmdMergeFile()
{
}


Long_t TCmdMergeFile::Cmd(vector<TString> args)
{
   args.erase(args.begin());
   if (args.size() < 2) {
      Help();
      return 1;
   }
   TString output = args.front();
   args.erase(args.begin());
   Run(output.Data(),args);
   return 1;
}

Long_t TCmdMergeFile::Run(const char* output, std::vector<TString> files)
{
   TFileMerger merger(kFALSE,kFALSE);

   merger.SetMsgPrefix("artemis_hadd");
   merger.SetPrintLevel(0);
   merger.OutputFile(output,1,0);
   
   std::vector<TString> paths;
   std::vector<Long_t> loaded;
   for (Int_t i = 0, n = files.size(); i < n; ++i) {
      TString file = files[i];
      TObjArray* tmppaths = gSystem->GetFromPipe(Form("ls %s",file.Data())).Tokenize("\n");
      for (Int_t ip = 0, np = tmppaths->GetEntries(); ip < np; ++ip) {
         TString filename =  ((TObjString*)tmppaths->UncheckedAt(ip))->GetString();
         FileStat_t fstat;
         gSystem->GetPathInfo(filename, fstat);
         bool isLoaded = false;
         for (std::vector<Long_t>::iterator it = loaded.begin(); it != loaded.end(); ++it) {
            if (*it == fstat.fIno) {
               isLoaded = true;
               break;
            }
         }
         if (!isLoaded) {
            paths.push_back(filename);
         }
      }
   }

   for (Int_t i = 0, n = paths.size(); i < n; ++i) {
      merger.AddFile(paths[i]);
   }
   merger.Merge();
   return 1;
}
