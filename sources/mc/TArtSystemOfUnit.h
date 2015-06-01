/* $Id:$ */
/**
 * @file   TArtSystemOfUnit.h
 * @date   Created : Feb 18, 2011 18:18:18 JST
 *   Last Modified : 2015-05-30 06:44:35 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTSYSTEMOFUNIT_H
#define TARTSYSTEMOFUNIT_H

namespace TArtSystemOfUnit {
// length : base unit = mm (millimeter)
   const double mm = 1.;
   const double m  = 1000.*mm;
   const double cm = 10.*mm;
   const double um = 1.E-6*m;
   const double nm = 1.E-9*m;
// time : base unit = ns (nanosecond)
   const double ns = 1.;
   const double us = 1000.*ns;
   const double s = 1.E+9*ns;
   const double ms = 1.E+6*ns;
// angle : base unit = rad (radian)
   const double rad = 1.;
   const double mrad = 1.E-3;
   const double deg  = 3.1415926535 / 180.;
// energy : base unit = MeV (megaelectronvolt);
   const double  eV = 1.E-6;
   const double keV = 1.E-3;
   const double MeV = 1.;
   const double GeV = 1.E+3;
};
#endif // end of #ifdef TARTUNIT_H
