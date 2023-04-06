/**
 * @file   constant_test.cc
 * @brief
 *
 * @date   Created:       2013-11-11 17:40:47
 *         Last Modified: 2013-11-11 17:43:49
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "constant.h"
#include "gtest/gtest.h"

using art::IsValid;
using art::kInvalidI;
using art::kInvalidD;

TEST(constant,IsValid) {
   EXPECT_TRUE(IsValid(0));
   EXPECT_TRUE(IsValid(-299));
   EXPECT_TRUE(IsValid(0.));
   EXPECT_TRUE(IsValid(32.35));
   EXPECT_FALSE(IsValid(kInvalidI));
   EXPECT_FALSE(IsValid(kInvalidD));
}
