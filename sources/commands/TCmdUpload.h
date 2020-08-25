/**
 * @file   TCmdUpload.h
 * @brief  Upload image file to GitLab via API
 *
 * @date   Created       : 2020-04-30 22:14:36 JST
 *         Last Modified : 2020-06-30 09:56:38 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2020 Shoichiro Masuoka
 */
/*

Upload image which is saved immediately before using TCmdSave (save command) to GitLab via API. 

- Settings

1. sign in to your GitLab account and open the user settings.
2. choose "Access Tokens" and create token with API scopes.
3. copy token and paste to a file (ex. .gitlab_api_token). Strongly recommend to add .gitignore this file if you put it into the git control directory.
4. remember your project ID to put image.
5. (option) if you want to copy to clipboard for returned markdown URL, install the clipboard access app. (ex. xsel, xclip, etc...)
6. add 'artemislogon.C' like,

   TCatCmdFactory *cf = TCatCmdFactory::Instance();
   // ...... (add the other commands) ......
   art::TCmdSave *cmdsave = art::TCmdSave::Instance();
   cmdsave->SetDefaultDirectory("figs") // Do NOT include ~ charactor when you use upload command. If you specify the home directory, please write by absolute path.
   cmdsave->SetAddDateDir(kTRUE);
   cmdsave->SetAutoName(kTRUE);
   cmdsave->AddFormat("png",1); // specify '1' at the 2nd parameter
   // ...... (Add format)
   cf->Register(cmdsave);
   
   art::TCmdUpload *cmdupload = new art::TCmdUpload();
   cmdupload->SetUploadURL("(GitLab host)/api/v4/projects/(project ID)/uploads");
   cmdupload->SetPrivateTokenFile(".gitlab_api_token");
   // --- (option) command for copying to clipboard ---
   cmdupload->SetClipBoardAccessCommand("xsel -bi");
   cmdupload->UseTmux(kTRUE); // if you use tmux & want to copy to clipboard, please add
   // --- end option ---
   cf->Register(cmdupload);

- Usage

1. Draw histograms which you want to upload
2. type "save" command
   artemis [] save
3. type "upload" command after you save
   artemis [] upload
4. markdown will be printed (and copied to clipboard if you specify clipboard command)
5. paste it at issues, merge requests, milestones, Wiki, etc... on your project


 */
#ifndef INCLUDE_GUARD_UUID_91997D95_8F39_429D_8EB5_3A664BAF8424
#define INCLUDE_GUARD_UUID_91997D95_8F39_429D_8EB5_3A664BAF8424

#include "TCatCmd.h"

#include <TString.h>
#include <vector>

namespace art {
   class TCmdUpload;
}

class art::TCmdUpload : public TCatCmd {
public:
   TCmdUpload();
   virtual ~TCmdUpload();


   virtual Long_t Cmd(vector<TString>);
   virtual Long_t Run(const char* filename = NULL);

   virtual void UploadFile(const TString&);
   virtual void PrintMarkdown(const TString&);
   virtual void CopyToClipBoard(const TString&);
   
   virtual void SetClipBoardAccessCommand(const TString command) { fClipBoardAccessCommand = command; }
   virtual const char* GetClipBoardAccessCommand() { return fClipBoardAccessCommand.Data(); }

   virtual void SetUploadURL(const TString url) { fUploadURL = url; }
   virtual const char* GetUploadURL() { return fUploadURL.Data(); }

   virtual void SetPrivateTokenFile(const TString token) { fPrivateTokenFile = token; }
   virtual const char* GetPrivateTokenFile() { return fPrivateTokenFile.Data(); }

   virtual void UseTmux(const Bool_t tmux=kFALSE) { fUseTmux = tmux; }
   virtual const Bool_t GetUseTmux() { return fUseTmux; }
  
protected:

private:

   TString fPrivateTokenFile; // set gitlab access token file.
   TString fUploadURL; // set upload URL.
   TString fClipBoardAccessCommand; // execute clip board command. Ex. 'xclip -selection c', 'xsel -bi'
   Bool_t fUseTmux; // if you use tmux & want to copy to clipboard, switch to kTRUE. (default: kFALSE)
   
   ClassDef(TCmdUpload,1) // Upload image file to GitLab via API
};

#endif // INCLUDE_GUARD_UUID_91997D95_8F39_429D_8EB5_3A664BAF8424
