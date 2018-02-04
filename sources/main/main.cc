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
#include "TString.h"
#include <TLoopManager.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#ifdef USE_MPI
#include <mpi.h>
#endif
#endif

int main (int argc, char **argv)
{
#ifdef USE_MPI
  // check option
  char opt;
  bool flag;
  TString filename;
  while ((opt = getopt(argc,argv,"i:")) != -1) {
    switch(opt) {
    case 'i':
      flag = 1;
      filename = optarg;
      break;
    default:
      break;
    }
  }
  if (flag) {
    int myrank, npe;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npe);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    art::TArtRint *theApp = new art::TArtRint(&argc, argv);
    theApp->Run();
    //    art::TLoopManager::Instance()->Add(filename);
    printf("myrank = %d / %d\n",myrank,npe); 
    MPI_Finalize();
  } else {
    art::TArtRint *theApp = new art::TArtRint(&argc, argv);
    theApp->Run();
  }

#else
   art::TArtRint *theApp = new art::TArtRint(&argc, argv);
   theApp->Run();
#endif

   return 0;
}

