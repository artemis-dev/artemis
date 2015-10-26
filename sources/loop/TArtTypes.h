/* $Id:$ */
/**
 * @file   TArtTypes.h
 * @date   Created : May 18, 2012 14:18:02 JST
 *   Last Modified : Oct 26, 2015 17:29:32 EDT
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
