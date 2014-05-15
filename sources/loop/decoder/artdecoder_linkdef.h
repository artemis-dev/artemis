/* $Id:$ */
/**
 * @file   artdecoder_linkdef.h
 * @date   Created : Jul 07, 2013 22:48:06 JST
 *   Last Modified : May 16, 2014 00:50:00 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ARTDECODER_LINKDEF_H
#define ARTDECODER_LINKDEF_H

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;


//#pragma link C++ class art::TModuleDecoderV1190-;
#pragma link C++ class art::TRawDataV1190;
#pragma link C++ class art::TModuleDecoderV1740;
#pragma link C++ class art::TRawDataFadc;
#pragma link C++ class art::TModuleDecoderSkip;
#pragma link C++ class art::TModuleDecoderV767;
#pragma link C++ class art::TModuleDecoderSIS3610;
#pragma link C++ class art::TModuleDecoderSIS3820;
//#pragma link C++ typedef art::TRawDataV767;
#pragma link C++ class art::TModuleDecoderV7XX;
#pragma link C++ class art::TModuleDecoderFixed<UChar_t>;
#pragma link C++ class art::TModuleDecoderFixed<UShort_t>;
#pragma link C++ class art::TModuleDecoderFixed<UInt_t>;
#pragma link C++ class art::TModuleDecoderFixed<ULong64_t>;
#pragma link C++ class art::TModuleDecoderV1190;

#endif /* __CINT__ */

#endif // end of #ifdef ARTDECODER_LINKDEF_H
