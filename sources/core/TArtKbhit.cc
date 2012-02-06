/* $Id:$ */
/**
 * @file   TArtKbhit.cc
 * @date   Created : Jan 10, 2011 01:10:22 JST
 *   Last Modified : Jan 10, 2011 01:50:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtKbhit.h"

#include <term.h>
#include <curses.h>
#include <unistd.h>

TArtKbhit::TArtKbhit()
   : fIsInit(0) 
{
}

TArtKbhit::~TArtKbhit()
{
   Close();
}

void TArtKbhit::Init()
{
   if (!fIsInit) {
      tcgetattr(0, &fInitial);
      fCurrent = fInitial;
      fCurrent.c_lflag &= ~ICANON;
      fCurrent.c_lflag &= ~ECHO;
      fCurrent.c_lflag &= ~ISIG;
      fCurrent.c_cc[VMIN] = 0;
      fCurrent.c_cc[VTIME] = 0;
      tcsetattr(0, TCSANOW, &fInitial);
   }
   fIsInit = 1;
}

void TArtKbhit::Close()
{
   if (fIsInit) {
      tcsetattr(0, TCSANOW, &fInitial);
      fIsInit = 0;
   } 
}

// check if enter is input or not
int TArtKbhit::ChkNL()
{
   while (Kbhit()) {
      int ch = Readch();
      if (ch == '\n') {
         return 1;
      }
   }
   return 0;
}

int TArtKbhit::Kbhit()
{
   char ch;
   int nread;
   if (!fIsInit) Init();

   if(fPeek != -1)
      return 1;
   fCurrent.c_cc[VMIN]=0;
   tcsetattr(0, TCSANOW, &fCurrent);
   nread = read(0, &ch, 1);
   fCurrent.c_cc[VMIN]=1;
   tcsetattr(0, TCSANOW, &fCurrent);

   if(nread == 1) {
      fPeek = ch;
      return 1;
   }
   return 0;
}

int TArtKbhit::Readch() 
{
   char ch;
   if (!fIsInit) Init();
   if(fPeek != -1) {
      ch = fPeek;
      fPeek = -1;
      return ch;
   }
   read(0, &ch, 1);
   return ch;
}
