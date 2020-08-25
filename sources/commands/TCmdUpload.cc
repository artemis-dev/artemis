/**
 * @file   TCmdUpload.cc
 * @brief  Upload image file to GitLab via API
 *
 * @date   Created       : 2020-04-30 22:13:55 JST
 *         Last Modified : 2020-06-30 10:04:14 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2020 Shoichiro Masuoka
 */

#include "TCmdUpload.h"
#include "TCmdSave.h"

#include <TSystem.h>
#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObject.h>
#include <TObjString.h>
#include <TString.h>

using art::TCmdUpload;

ClassImp(TCmdUpload)

TCmdUpload::TCmdUpload()
//: fJSONResult("")
{
   SetName("upload");
   SetTitle("Upload image file to GitLab via API");
}

TCmdUpload::~TCmdUpload()
{
}


Long_t TCmdUpload::Cmd(vector<TString> token)
{
   if (token.size() > 1) {
      return Run(token[1].Data());
   }
   return Run();
}

Long_t TCmdUpload::Run(const char* filename_input)
{
   const TString &filename = filename_input ? filename_input : TCmdSave::Instance()->GetPrintFileName();
   if (filename.IsNull()) {
      Error("Run","There is no file to be uploaded.");
      return 1;
   }
   if (GetUploadURL() != NULL && GetPrivateTokenFile() != NULL) {
      Info("Run",TString::Format("upload image file: %s to %s",filename.Data(),GetUploadURL()));
      UploadFile(filename);
   } else {
      Error("Run","Upload URL and/or private access token file are not specified");
      return 1;
   }
   return 1;
}

void TCmdUpload::UploadFile(const TString &fileName)
{
   TString token = gSystem->GetFromPipe(TString::Format("cat %s",GetPrivateTokenFile()));
   if (token.IsNull()) {
      Error("UploadFile","invalid token file");
      return;
   }
   TString sendAPI = TString::Format("curl --request POST --header 'PRIVATE-TOKEN: %s' --form 'file=@%s' %s", token.Data(),fileName.Data(),GetUploadURL());

   const TString json = gSystem->GetFromPipe(sendAPI);
   
   PrintMarkdown(json);
}

void TCmdUpload::PrintMarkdown(const TString &json)
{
//   const TString json = GetJSONResult();
   if (json.IsNull()) {
      Error("PrintMarkdown","failed to upload");
   } else {
      TObjArray *resultList = TPRegexp("\"markdown\":\"(.*)\"").MatchS(json);
      TObject *resultPtr = (TObjString*)resultList->At(1);
      if (resultPtr == NULL) {
         Error("PrintMarkdown", TString::Format("failed to upload: JSON parse error\n %s",json.Data()));
         printf("suggestion-> 401: invalid access token, 404: invalid URL\n");
         return;
      } else {
         TString result = ((TObjString*)resultList->At(1))->GetString();
         printf("Upload completed. Markdown: \n%s\n",result.Data());
         CopyToClipBoard(result);
      }
   }
}

void TCmdUpload::CopyToClipBoard(const TString &content)
{
   TString command = GetClipBoardAccessCommand();
   if (command.IsNull()) {
      Info("CopyToClipBoard","clip board command is not specified");
      return;
   } else {
      if (GetUseTmux()) {
         TString display = gSystem->GetFromPipe("tmux showenv DISPLAY");
         gSystem->Exec(TString::Format("echo '%s' | env %s %s",
                                       content.Data(),display.Data(),GetClipBoardAccessCommand()));
      } else {
      gSystem->Exec(TString::Format("echo '%s' | %s"
                                    ,content.Data(),GetClipBoardAccessCommand()));
      }
   }
}
