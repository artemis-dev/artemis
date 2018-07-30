/**
 * @file   TArtemisUtil.h
 * @brief  utilities
 *
 * @date   Created       : 2018-07-26 16:30:08 JST
 *         Last Modified : 2018-07-30 12:43:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_C4FCAA21_9C8F_4673_AF74_3F6D6903E698
#define INCLUDE_GUARD_UUID_C4FCAA21_9C8F_4673_AF74_3F6D6903E698

#include <Rtypes.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


namespace art {
   namespace Util {

      // directory operation
      Bool_t PrepareDirectoryFor(const char* path);

      // swap values
      template<class T> void Swap(T& val1, T& val2);

      
      // file merger in mpi
      Bool_t MPIFileMerger(const char* file);
   }
}

template<class T> 
void art::Util::Swap(T& val1, T& val2)
{
   T tmp = val1;
   val1 = val2;
   val2 = tmp;
}

#endif // INCLUDE_GUARD_UUID_C4FCAA21_9C8F_4673_AF74_3F6D6903E698
