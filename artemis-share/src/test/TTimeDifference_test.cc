/**
 * @file   TTimeDifference_test.cc
 * @brief
 *
 * @date   Created:       2013-11-29 10:34:33
 *         Last Modified: 2013-11-29 13:26:30
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimeDifference.h"
#include "TTimingData.h"
#include "constant.h"
#include "gtest/gtest.h"

#include <TRandom.h>

using art::TTimeDifference;
using art::TTimingData;
using art::kInvalidD;
using art::IsValid;

namespace {
   const Int_t NDATA = 1;
   const Int_t NDOUBLE = 1;
   const Int_t NINT = 4;
   const Double_t MAX = 1e+8;
   const UInt_t SEED = 43975;
   enum intID {kHitID,kHitID0,kDetID,kDetID0};
}

class TTimeDifferenceTest : public ::testing::Test {
public:
   virtual void SetUp() {
      gRandom->SetSeed(SEED);

      for(Int_t i = 0; i!= NDATA; ++i) {
	 for(Int_t j = 0; j!= NDOUBLE; ++j) {
	 randomDouble[i][j] = gRandom->Uniform(-MAX,MAX);
	 }	 

	 for(Int_t j = 0; j!= NINT; ++j) {
	    randomInt[i][j] = gRandom->Integer((Int_t)MAX) - (Int_t)MAX/2;
	 }
      }

      for(Int_t i = 0; i!= NDATA; ++i) {
	 randomData[i].SetHitID(randomInt[i][kHitID]);
	 randomData[i].SetHitID0(randomInt[i][kHitID0]);
	 randomData[i].SetDetID(randomInt[i][kDetID]);
	 randomData[i].SetDetID0(randomInt[i][kDetID0]);
	 randomData[i].SetTiming(randomDouble[i][0]);
      }
   }

//   virtual void TearDown() {}

protected:
   Double_t randomDouble[NDATA][NDOUBLE];
   Int_t randomInt[NDATA][NINT];
   const TTimeDifference default_data;
   TTimeDifference randomData[NDATA];
};


TEST_F(TTimeDifferenceTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetTiming()));
   EXPECT_FALSE(IsValid(default_data.GetHitID()));
   EXPECT_FALSE(IsValid(default_data.GetHitID0()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
   EXPECT_FALSE(IsValid(default_data.GetDetID0()));
}

TEST_F(TTimeDifferenceTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      randomData[i].Clear();
      EXPECT_FALSE(IsValid(randomData[i].GetTiming()));
      EXPECT_FALSE(IsValid(randomData[i].GetHitID()));
      EXPECT_FALSE(IsValid(randomData[i].GetHitID0()));
      EXPECT_FALSE(IsValid(randomData[i].GetDetID()));
      EXPECT_FALSE(IsValid(randomData[i].GetDetID0()));
   }
}

TEST_F(TTimeDifferenceTest,SetterGetter) {
   for(Int_t i =0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(randomDouble[i][0],randomData[i].GetTiming());
      EXPECT_EQ(randomInt[i][kHitID],randomData[i].GetHitID());
      EXPECT_EQ(randomInt[i][kHitID0],randomData[i].GetHitID0());
      EXPECT_EQ(randomInt[i][kDetID],randomData[i].GetDetID());
      EXPECT_EQ(randomInt[i][kDetID0],randomData[i].GetDetID0());
   }
}

TEST_F(TTimeDifferenceTest,CopyConstructor) {
   const TTimeDifference default_data_copied(default_data);
   EXPECT_FALSE(IsValid(default_data_copied.GetTiming()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID0()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID0()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimeDifference data_copied(randomData[i]);
      EXPECT_DOUBLE_EQ(randomDouble[i][0],data_copied.GetTiming());
      EXPECT_EQ(randomInt[i][kHitID],data_copied.GetHitID());
      EXPECT_EQ(randomInt[i][kHitID0],data_copied.GetHitID0());
      EXPECT_EQ(randomInt[i][kDetID],data_copied.GetDetID());
      EXPECT_EQ(randomInt[i][kDetID0],data_copied.GetDetID0());
   }
}

TEST_F(TTimeDifferenceTest,AssignmentOperator) {
   const TTimeDifference default_data_copied = default_data;
   EXPECT_FALSE(IsValid(default_data_copied.GetTiming()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID0()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID0()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimeDifference data_copied = randomData[i];
      EXPECT_DOUBLE_EQ(randomDouble[i][0],data_copied.GetTiming());
      EXPECT_EQ(randomInt[i][kHitID],data_copied.GetHitID());
      EXPECT_EQ(randomInt[i][kHitID0],data_copied.GetHitID0());
      EXPECT_EQ(randomInt[i][kDetID],data_copied.GetDetID());
      EXPECT_EQ(randomInt[i][kDetID0],data_copied.GetDetID0());
   }
}

TEST_F(TTimeDifferenceTest, CopyMethod) {
   TTimeDifference default_data_copied;
   default_data.Copy(default_data_copied);
   EXPECT_FALSE(IsValid(default_data_copied.GetTiming()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetHitID0()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID()));
   EXPECT_FALSE(IsValid(default_data_copied.GetDetID0()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TTimeDifference data_copied;
      randomData[i].Copy(data_copied);
      EXPECT_DOUBLE_EQ(randomDouble[i][0],data_copied.GetTiming());
      EXPECT_EQ(randomInt[i][kHitID],data_copied.GetHitID());
      EXPECT_EQ(randomInt[i][kHitID0],data_copied.GetHitID0());
      EXPECT_EQ(randomInt[i][kDetID],data_copied.GetDetID());
      EXPECT_EQ(randomInt[i][kDetID0],data_copied.GetDetID0());
   }
}

TEST_F(TTimeDifferenceTest,Compare) {
   TTimeDifference a,b;
   a.SetTiming(1.);
   a.SetDetID(1);

   b.SetTiming(1.);
   b.SetDetID(1);   

   const Int_t SMALL = -548;
   const Int_t LARGE = 430;

   TTimeDifference::SetSortType(TTimeDifference::kID);
   TTimeDifference::SetSortOrder(TTimeDifference::kASC);
   b.SetDetID(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimeDifference::SetSortOrder(TTimeDifference::kDESC);
   b.SetDetID(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TTimeDifference::SetSortType(TTimeDifference::kTiming);
   TTimeDifference::SetSortOrder(TTimeDifference::kASC);
   b.SetTiming(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimeDifference::SetSortOrder(TTimeDifference::kDESC);
   b.SetTiming(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   
}

TEST_F(TTimeDifferenceTest,CompareWithInvalid) {
   TTimeDifference a;
   a.SetTiming(1.);
   a.SetDetID(1);

   TTimeDifference::SetSortType(TTimeDifference::kID);
   TTimeDifference::SetSortOrder(TTimeDifference::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimeDifference::SetSortOrder(TTimeDifference::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimeDifference::SetSortType(TTimeDifference::kTiming);
   TTimeDifference::SetSortOrder(TTimeDifference::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimeDifference::SetSortOrder(TTimeDifference::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}

TEST_F(TTimeDifferenceTest,TakeDifference) {
   const Double_t T0 = 9245.2434;
   const Double_t T1 = 5474.23254;
   TTimingData *start = new TTimingData;
   start->SetTiming(T0);
   TTimingData *stop = new TTimingData;
   stop->SetTiming(T1);

   TTimeDifference diff;
   diff.TakeDifference(start,stop);

   EXPECT_DOUBLE_EQ(T0-T1,diff);
}
