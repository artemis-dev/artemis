/* $Id:$ */
/**
 * @file   TTypes.h
 * @date   Created : May 18, 2012 14:18:02 JST
 *   Last Modified : May 18, 2012 14:37:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TTYPES_H
#define ART_TTYPES_H

#include <vector> 
class TString;
namespace art {
   typedef std::vector<TString> StringVec_t;
   typedef std::vector<float>   FloatVec_t;
   typedef std::vector<int>     IntVec_t;
}

#endif // end of #ifdef TTYPES_H
