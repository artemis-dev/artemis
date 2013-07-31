/* $Id:$ */
/**
 * @file   TRawDataSimple.cc
 * @date   Created : Jul 22, 2013 18:22:37 JST
 *   Last Modified : 2013-07-31 16:19:36
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRawDataSimple.h"

templateClassImp(art::TRawDataSimple);

using art::TRawDataSimple;

template <typename T> art::TRawDataSimple<T>::TRawDataSimple()
   : fValue(TRawDataObject::kInvalid)
{
}
template <typename T> art::TRawDataSimple<T>::~TRawDataSimple()
{
}

// explicit instantiations
template class TRawDataSimple<UChar_t>;
template class TRawDataSimple<UShort_t>;
template class TRawDataSimple<UInt_t>;
template class TRawDataSimple<ULong64_t>;
