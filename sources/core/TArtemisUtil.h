/**
 * @file   TArtemisUtil.h
 * @brief  utilities
 *
 * @date   Created       : 2018-07-26 16:30:08 JST
 *         Last Modified : 2018-09-05 13:44:04 JST (ota)
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

      // Load analysis information (at top directory of this directory)
      Bool_t LoadAnalysisInformation(const char *infoName = NULL);

      /// \brief get polygon vertexes
      /// The polygon is a closed one, which request the size of output vertexes should be n + 1, where n is the number of vertexes
      /// @param[in] n number of vertex
      /// @param[in] side length of side 
      /// @param[in] angle rotation angle (rad)
      /// @param[out] vx x coordinates of vertex
      /// @param[out] vy y coordinates of vertex
      /// @param[in] xoffset offset of x coordinate
      /// @param[in] yoffset offset of y coordinate
      void GetPolygonFromSide(Int_t n, Double_t side, Double_t angle, Double_t *vx, Double_t *vy, Double_t xoffset = 0., Double_t yoffset = 0);
      

#ifdef USE_MPI
      // file merger in mpi
      Bool_t MPIFileMerger(const char* file);
#endif
      
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
