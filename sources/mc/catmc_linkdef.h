/* $Id: LinkDefArtMC.h 44 2011-02-16 14:39:26Z sweetbluemoon $ */
/**
 * @file   LinkDefArtMC.h
 * @date   Created : Jan 25, 2011 21:25:03 JST
 *   Last Modified : 2020-10-04 18:02:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef LINKDEFARTMC_H
#define LINKDEFARTMC_H

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class TArtEventCollections+;
#pragma link C++ class TArtParticle+;
#pragma link C++ class TCatTwoBodyKinematics;
#pragma link C++ class TArtAtomicMassTable;
#pragma link C++ class art::TBinaryReactionGenerator;
#pragma link C++ global gAtomicMassTable;
#pragma link C++ class art::TTrackG4;
#pragma link C++ class art::TRandomGraph;
#endif

#endif // end of #ifdef LINKDEFARTMC_H
