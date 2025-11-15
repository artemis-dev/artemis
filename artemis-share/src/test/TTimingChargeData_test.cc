/**
 * @file   TTimingChargeData_test.cc
 * @brief
 *
 * @date   Created:       2013-11-11 15:15:03
 *         Last Modified: 2013-11-29 10:14:14
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingChargeData.h"
#include "constant.h"
#include "gtest/gtest.h"

using art::TTimingChargeData;
using art::kInvalidD;
using art::IsValid;

namespace {
   static const Int_t NDATA = 3;  
   const Int_t valI[NDATA] = {0,-43,4363};
   const Double_t valD[NDATA] = {0.,-46.34,967.3};
}

class TTimingChargeDataTest : public ::testing::Test {
public:
   virtual void SetUp() {
      for(Int_t i = 0; i!= NDATA; ++i) {
	 data[i].SetTiming(valD[i]);
	 data[i].SetCharge(valD[i]);
	 data[i].SetDetID(valI[i]);
      }
   }

//   virtual void TearDown() {}

protected:
   const TTimingChargeData default_data;
   TTimingChargeData data[NDATA];
};

TEST_F(TTimingChargeDataTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetTiming()));
   EXPECT_FALSE(IsValid(default_data.GetCharge()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
}

TEST_F(TTimingChargeDataTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      data[i].Clear();
      EXPECT_FALSE(IsValid(data[i].GetTiming()));
      EXPECT_FALSE(IsValid(data[i].GetCharge()));
      EXPECT_FALSE(IsValid(data[i].GetDetID()));
   }
}

TEST_F(TTimingChargeDataTest,SetterGetter) {
   for(Int_t i = 0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(valD[i],data[i].GetTiming());
      EXPECT_DOUBLE_EQ(valD[i],data[i].GetCharge());
      EXPECT_EQ(valI[i],data[i].GetDetID());

      TTimingChargeData cdata;
      cdata.SetTiming(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());
      cdata.SetCharge(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());
   }
}

TEST_F(TTimingChargeDataTest,CopyConstructor) {
   const TTimingChargeData bad_cdata(default_data);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimingChargeData cdata(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TTimingChargeDataTest,AssignmentOperator) {
   const TTimingChargeData bad_adata = default_data;
   EXPECT_FALSE(IsValid(bad_adata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_adata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_adata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimingChargeData adata = data[i];
      EXPECT_DOUBLE_EQ(valD[i],adata.GetTiming());
      EXPECT_DOUBLE_EQ(valD[i],adata.GetCharge());
      EXPECT_EQ(valI[i],adata.GetDetID());
   }
}

TEST_F(TTimingChargeDataTest,CopyMethod) {
   TTimingChargeData bad_cdata;
   default_data.Copy(bad_cdata);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TTimingChargeData cdata;
      data[i].Copy(cdata);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TTimingChargeDataTest,Compare) {
   TTimingChargeData a,b;
   a.SetDetID(1);
   a.SetTiming(1.);
   a.SetCharge(1.);

   b.SetDetID(1);
   b.SetTiming(1.);
   b.SetCharge(1.);

   const Int_t SMALL = -548;
   const Int_t LARGE = 430;

   TTimingChargeData::SetSortType(TTimingChargeData::kID);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   b.SetDetID(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   b.SetDetID(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TTimingChargeData::SetSortType(TTimingChargeData::kTiming);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   b.SetTiming(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   b.SetTiming(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TTimingChargeData::SetSortType(TTimingChargeData::kCharge);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   b.SetCharge(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   b.SetCharge(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   
}

TEST_F(TTimingChargeDataTest,CompareWithInvalid) {
   TTimingChargeData a;
   a.SetTiming(1.);
   a.SetCharge(1.);
   a.SetDetID(1);

   TTimingChargeData::SetSortType(TTimingChargeData::kID);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingChargeData::SetSortType(TTimingChargeData::kTiming);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingChargeData::SetSortType(TTimingChargeData::kCharge);
   TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}
