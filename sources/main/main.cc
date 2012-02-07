/* $Id:$ */
/**
 * @file   main.cc
 * @date   Created : Feb 06, 2012 00:06:56 JST
 *   Last Modified : Feb 06, 2012 00:42:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"

int main (int argc, char **argv)
{
   TArtRint *theApp = new TArtRint(&argc, argv);
   theApp->Run();
   return 0;
}

