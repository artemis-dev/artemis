/* @file TResponseFunctionNArray.cc
 * @brief responsee function using TNArray
 *
 * @date Create        : 2020-05-27 14:28:01 JST
 *       Last Modified : 2020-05-27 14:31:51 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TResponseFunctionNArray.h"

ClassImp(art::TResponseFunctionNArray)

using art::TResponseFunctionNArray;

TResponseFunctionNArray::TResponseFunctionNArray()
   : TNArray("table","response table","angle:distance:deviation:value")
{
   Add("angle",0.,60.,301);
   Add("distance",EncodeDistance(0.),EncodeDistance(20.),201);
   Add("deviation",0.,2.5,251);
}

TResponseFunctionNArray::~TResponseFunctionNArray()
{
}

