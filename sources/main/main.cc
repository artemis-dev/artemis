/* $Id:$ */
/**
 * @file   main.cc
 * @date   Created : Feb 06, 2012 00:06:56 JST
 *   Last Modified : May 05, 2014 07:12:46 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"

int main (int argc, char **argv)
{
   art::TArtRint *theApp = new art::TArtRint(&argc, argv);
   theApp->Run();
   return 0;
}

