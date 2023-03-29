/**
 * @file   TAffineConverter_test.cc
 * @brief
 *
 * @date   Created:       2013-11-11 17:10:52
 *         Last Modified: 2013-11-11 18:31:48
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TAffineConverter.h"
#include "constant.h"
#include "gtest/gtest.h"

using art::TAffineConverter;
using art::IsValid;
using art::kInvalidD;

TEST(TAffineConverter,DefaultConstructor) {
   const TAffineConverter conv;

   EXPECT_DOUBLE_EQ(0.,conv.Convert(0.));
   EXPECT_DOUBLE_EQ(1.,conv.Convert(1.));
   EXPECT_FALSE(IsValid(conv.Convert(kInvalidD)));
}

TEST(TAffineConverter,Conversion) {
   TAffineConverter conv;
   conv.Set(3.,2.);

   EXPECT_DOUBLE_EQ(3.,conv.Convert(0.));
   EXPECT_DOUBLE_EQ(5.,conv.Convert(1.));
   EXPECT_DOUBLE_EQ(-197.,conv.Convert(-100.));
   EXPECT_FALSE(IsValid(conv.Convert(kInvalidD)));
}

