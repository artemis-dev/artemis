/* $Id:$ */
/**
 * @file   TArtKbhit.h
 * @date   Created : Jan 10, 2011 01:10:18 JST
 *   Last Modified : Jan 31, 2012 14:41:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTLOOPCHK_H
#define TARTLOOPCHK_H

#include <termios.h>

class TArtKbhit  {

public:
   TArtKbhit();
   virtual ~TArtKbhit();

   int ChkNL();
   int Kbhit();
   int Readch();

protected:
   virtual void Init();
   virtual void Close();

private:
   struct termios fInitial;
   struct termios fCurrent;
   int    fPeek;
   int    fIsInit;
};

#endif // end of #ifdef TARTLOOPCHK_H
