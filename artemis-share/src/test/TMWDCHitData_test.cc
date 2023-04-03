/**
 * @file   TMWDCHitData_test.cc
 * @brief
 *
 * @date   Created:       2013-11-29 11:39:59
 *         Last Modified: 2013-11-29 12:40:19
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCHitData.h"
#include "constant.h"
#include "gtest/gtest.h"
#include <TRandom.h>

using art::TMWDCHitData;
using art::kInvalidD;
using art::IsValid;

namespace {
   const Int_t NDATA = 1;
   const Int_t NINT = 1;
   const Int_t NDOUBLE = 3;
   const Double_t MAX = 1e+8;
   const UInt_t SEED = 43975;
   enum member {kTiming,kCharge,kDriftLength};
}

class TMWDCHitDataTest : public ::testing::Test {
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
	 randomData[i].SetTiming(randomDouble[i][kTiming]);
	 randomData[i].SetCharge(randomDouble[i][kCharge]);
	 randomData[i].SetDriftLength(randomDouble[i][kDriftLength]);
	 randomData[i].SetDetID(randomInt[i][0]);
      }
   }

//   virtual void TearDown() {}

protected:
   Double_t randomDouble[NDATA][NDOUBLE];
   Int_t randomInt[NDATA][NINT];
   const TMWDCHitData default_data;
   TMWDCHitData randomData[NDATA];
};

TEST_F(TMWDCHitDataTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetTiming()));
   EXPECT_FALSE(IsValid(default_data.GetCharge()));
   EXPECT_FALSE(IsValid(default_data.GetDriftLength()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
}

TEST_F(TMWDCHitDataTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      randomData[i].Clear();
      EXPECT_FALSE(IsValid(randomData[i].GetTiming()));
      EXPECT_FALSE(IsValid(randomData[i].GetCharge()));
      EXPECT_FALSE(IsValid(randomData[i].GetDriftLength()));
      EXPECT_FALSE(IsValid(randomData[i].GetDetID()));
   }
}

TEST_F(TMWDCHitDataTest,SetterGetter) {
   for(Int_t i = 0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(randomDouble[i][kTiming],randomData[i].GetTiming());
      EXPECT_DOUBLE_EQ(randomDouble[i][kCharge],randomData[i].GetCharge());
      EXPECT_DOUBLE_EQ(randomDouble[i][kDriftLength],randomData[i].GetDriftLength());
      EXPECT_EQ(randomInt[i][0],randomData[i].GetDetID());
   }
}

TEST_F(TMWDCHitDataTest,CopyConstructor) {
   const TMWDCHitData bad_cdata(default_data);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDriftLength()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TMWDCHitData cdata(randomData[i]);
      EXPECT_DOUBLE_EQ(randomDouble[i][kTiming],cdata.GetTiming());
      EXPECT_DOUBLE_EQ(randomDouble[i][kCharge],cdata.GetCharge());
      EXPECT_DOUBLE_EQ(randomDouble[i][kDriftLength],cdata.GetDriftLength());
      EXPECT_EQ(randomInt[i][0],cdata.GetDetID());
   }
}

TEST_F(TMWDCHitDataTest,AssignmentOperator) {
   const TMWDCHitData bad_cdata = default_data;
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDriftLength()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TMWDCHitData cdata = randomData[i];
      EXPECT_DOUBLE_EQ(randomDouble[i][kTiming],cdata.GetTiming());
      EXPECT_DOUBLE_EQ(randomDouble[i][kCharge],cdata.GetCharge());
      EXPECT_DOUBLE_EQ(randomDouble[i][kDriftLength],cdata.GetDriftLength());
      EXPECT_EQ(randomInt[i][0],cdata.GetDetID());
   }
}

TEST_F(TMWDCHitDataTest,CopyMethod) {
   TMWDCHitData bad_cdata;
   default_data.Copy(bad_cdata);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDriftLength()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TMWDCHitData cdata;
      randomData[i].Copy(cdata);
      EXPECT_DOUBLE_EQ(randomDouble[i][kTiming],cdata.GetTiming());
      EXPECT_DOUBLE_EQ(randomDouble[i][kCharge],cdata.GetCharge());
      EXPECT_DOUBLE_EQ(randomDouble[i][kDriftLength],cdata.GetDriftLength());
      EXPECT_EQ(randomInt[i][0],cdata.GetDetID());
   }
}

TEST_F(TMWDCHitDataTest,Compare) {
   TMWDCHitData a,b;
   a.SetTiming(1.);
   a.SetCharge(1.);
   a.SetDriftLength(1.);
   a.SetDetID(1);

   b.SetTiming(1.);
   b.SetCharge(1.);
   b.SetDriftLength(1.);
   b.SetDetID(1);   

   const Int_t SMALL = -548;
   const Int_t LARGE = 430;

   TMWDCHitData::SetSortType(TMWDCHitData::kID);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   b.SetDetID(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   b.SetDetID(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TMWDCHitData::SetSortType(TMWDCHitData::kTiming);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   b.SetTiming(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   b.SetTiming(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TMWDCHitData::SetSortType(TMWDCHitData::kCharge);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   b.SetCharge(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   b.SetCharge(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TMWDCHitData::SetSortType(TMWDCHitData::kDriftLength);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   b.SetDriftLength(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDriftLength(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDriftLength(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   b.SetDriftLength(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDriftLength(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDriftLength(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   
}

TEST_F(TMWDCHitDataTest,CompareWithInvalid) {
   TMWDCHitData a;
   a.SetTiming(1.);
   a.SetCharge(1.);
   a.SetDriftLength(1.);
   a.SetDetID(1);

   TMWDCHitData::SetSortType(TMWDCHitData::kID);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortType(TMWDCHitData::kTiming);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortType(TMWDCHitData::kCharge);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortType(TMWDCHitData::kDriftLength);
   TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TMWDCHitData::SetSortOrder(TMWDCHitData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}
