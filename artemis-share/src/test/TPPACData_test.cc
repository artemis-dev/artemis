/**
 * @file   TPPACData_test.cc
 * @brief
 *
 * @date   Created:       2013-11-29 09:01:14
 *         Last Modified: 2014-01-08 10:23:27
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TPPACData.h"
#include "constant.h"
#include "gtest/gtest.h"
#include <TRandom.h>
#include <TMath.h>

using art::TPPACData;
using art::kInvalidD;
using art::IsValid;

namespace {
   const Int_t NDATA = 1;
   const Int_t NMEMBER = 10;
   const Double_t MAX = 1e+8;
   const UInt_t SEED = 43975;
   enum member {kTX1,kTX2,kTY1,kTY2,kTAnode,kQX1,kQX2,kQY1,kQY2,kQAnode};
}

class TPPACDataTest : public ::testing::Test {
public:
   virtual void SetUp() {
      gRandom->SetSeed(SEED);

      for(Int_t i = 0; i!= NDATA; ++i) {
	 for(Int_t j = 0; j!= NMEMBER; ++j) {
	 randomDouble[i][j] = gRandom->Uniform(-MAX,MAX);
	 }
	 randomInt[i][0] = gRandom->Integer((Int_t)MAX) - (Int_t)MAX/2;
      }

      for(Int_t i = 0; i!= NDATA; ++i) {
	 randomData[i].SetTX1(randomDouble[i][kTX1]);
	 randomData[i].SetTX2(randomDouble[i][kTX2]);
	 randomData[i].SetTY1(randomDouble[i][kTY1]);
	 randomData[i].SetTY2(randomDouble[i][kTY2]);
	 randomData[i].SetTAnode(randomDouble[i][kTAnode]);
	 randomData[i].SetQX1(randomDouble[i][kQX1]);
	 randomData[i].SetQX2(randomDouble[i][kQX2]);
	 randomData[i].SetQY1(randomDouble[i][kQY1]);
	 randomData[i].SetQY2(randomDouble[i][kQY2]);
	 randomData[i].SetQAnode(randomDouble[i][kQAnode]);
	 randomData[i].SetDetID(randomInt[i][0]);
	 randomData[i].Update();
      }
   }

//   virtual void TearDown() {}

protected:
   Double_t randomDouble[NDATA][NMEMBER];
   Int_t randomInt[NDATA][1];
   const TPPACData default_data;
   TPPACData randomData[NDATA];
};

TEST_F(TPPACDataTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetTX1()));
   EXPECT_FALSE(IsValid(default_data.GetTX2()));
   EXPECT_FALSE(IsValid(default_data.GetTY1()));
   EXPECT_FALSE(IsValid(default_data.GetTY2()));
   EXPECT_FALSE(IsValid(default_data.GetTAnode()));
   EXPECT_FALSE(IsValid(default_data.GetQX1()));
   EXPECT_FALSE(IsValid(default_data.GetQX2()));
   EXPECT_FALSE(IsValid(default_data.GetQY1()));
   EXPECT_FALSE(IsValid(default_data.GetQY2()));
   EXPECT_FALSE(IsValid(default_data.GetQAnode()));
   EXPECT_FALSE(IsValid(default_data.GetX()));
   EXPECT_FALSE(IsValid(default_data.GetY()));
   EXPECT_FALSE(IsValid(default_data.GetTXSum()));
   EXPECT_FALSE(IsValid(default_data.GetTYSum()));
   EXPECT_FALSE(IsValid(default_data.GetTXDiff()));
   EXPECT_FALSE(IsValid(default_data.GetTYDiff()));
   EXPECT_FALSE(IsValid(default_data.GetQXSum()));
   EXPECT_FALSE(IsValid(default_data.GetQYSum()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
}

TEST_F(TPPACDataTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      randomData[i].Clear();
      EXPECT_FALSE(IsValid(randomData[i].GetTX1()));
      EXPECT_FALSE(IsValid(randomData[i].GetTX2()));
      EXPECT_FALSE(IsValid(randomData[i].GetTY1()));
      EXPECT_FALSE(IsValid(randomData[i].GetTY2()));
      EXPECT_FALSE(IsValid(randomData[i].GetTAnode()));
      EXPECT_FALSE(IsValid(randomData[i].GetQX1()));
      EXPECT_FALSE(IsValid(randomData[i].GetQX2()));
      EXPECT_FALSE(IsValid(randomData[i].GetQY1()));
      EXPECT_FALSE(IsValid(randomData[i].GetQY2()));
      EXPECT_FALSE(IsValid(randomData[i].GetQAnode()));
      EXPECT_FALSE(IsValid(randomData[i].GetX()));
      EXPECT_FALSE(IsValid(randomData[i].GetY()));
      EXPECT_FALSE(IsValid(default_data.GetTXSum()));
      EXPECT_FALSE(IsValid(default_data.GetTYSum()));
      EXPECT_FALSE(IsValid(default_data.GetTXDiff()));
      EXPECT_FALSE(IsValid(default_data.GetTYDiff()));
      EXPECT_FALSE(IsValid(default_data.GetQXSum()));
      EXPECT_FALSE(IsValid(default_data.GetQYSum()));
      EXPECT_FALSE(IsValid(randomData[i].GetDetID()));
   }
}

TEST_F(TPPACDataTest,SetterGetter) {
   for(Int_t i = 0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX1],randomData[i].GetTX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX2],randomData[i].GetTX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY1],randomData[i].GetTY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY2],randomData[i].GetTY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTAnode],randomData[i].GetTAnode());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX1],randomData[i].GetQX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX2],randomData[i].GetQX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY1],randomData[i].GetQY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY2],randomData[i].GetQY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQAnode],randomData[i].GetQAnode());
      EXPECT_EQ(randomInt[i][0],randomData[i].GetDetID());
   }

   for(Int_t i = 0; i!=NDATA;++i) {
      TPPACData data;
      data.SetT(randomDouble[i][kTX1],0);
      data.SetT(randomDouble[i][kTX2],1);
      data.SetT(randomDouble[i][kTY1],2);
      data.SetT(randomDouble[i][kTY2],3);
      data.SetT(randomDouble[i][kTAnode],4);
      data.SetQ(randomDouble[i][kQX1],0);
      data.SetQ(randomDouble[i][kQX2],1);
      data.SetQ(randomDouble[i][kQY1],2);
      data.SetQ(randomDouble[i][kQY2],3);
      data.SetQ(randomDouble[i][kQAnode],4);
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX1],data.GetTX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX2],data.GetTX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY1],data.GetTY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY2],data.GetTY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTAnode],data.GetTAnode());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX1],data.GetQX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX2],data.GetQX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY1],data.GetQY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY2],data.GetQY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQAnode],data.GetQAnode());
   }
}

TEST_F(TPPACDataTest,CopyConstructor) {
   const TPPACData bad_cdata(default_data);
   EXPECT_FALSE(IsValid(bad_cdata.GetTX1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTX2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTY1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTY2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTAnode()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQX1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQX2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQY1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQY2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQAnode()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TPPACData cdata(randomData[i]);
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX1],cdata.GetTX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX2],cdata.GetTX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY1],cdata.GetTY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY2],cdata.GetTY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTAnode],cdata.GetTAnode());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX1],cdata.GetQX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX2],cdata.GetQX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY1],cdata.GetQY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY2],cdata.GetQY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQAnode],cdata.GetQAnode());
      EXPECT_EQ(randomInt[i][0],cdata.GetDetID());
   }
}

TEST_F(TPPACDataTest,AssignmentOperator) {
   const TPPACData bad_adata = default_data;
   EXPECT_FALSE(IsValid(bad_adata.GetTX1()));
   EXPECT_FALSE(IsValid(bad_adata.GetTX2()));
   EXPECT_FALSE(IsValid(bad_adata.GetTY1()));
   EXPECT_FALSE(IsValid(bad_adata.GetTY2()));
   EXPECT_FALSE(IsValid(bad_adata.GetTAnode()));
   EXPECT_FALSE(IsValid(bad_adata.GetQX1()));
   EXPECT_FALSE(IsValid(bad_adata.GetQX2()));
   EXPECT_FALSE(IsValid(bad_adata.GetQY1()));
   EXPECT_FALSE(IsValid(bad_adata.GetQY2()));
   EXPECT_FALSE(IsValid(bad_adata.GetQAnode()));
   EXPECT_FALSE(IsValid(bad_adata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TPPACData adata = randomData[i];
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX1],adata.GetTX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX2],adata.GetTX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY1],adata.GetTY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY2],adata.GetTY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTAnode],adata.GetTAnode());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX1],adata.GetQX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX2],adata.GetQX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY1],adata.GetQY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY2],adata.GetQY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQAnode],adata.GetQAnode());
      EXPECT_EQ(randomInt[i][0],adata.GetDetID());
   }
}

TEST_F(TPPACDataTest,CopyMethod) {
   TPPACData bad_cdata;
   default_data.Copy(bad_cdata);
   EXPECT_FALSE(IsValid(bad_cdata.GetTX1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTX2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTY1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTY2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetTAnode()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQX1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQX2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQY1()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQY2()));
   EXPECT_FALSE(IsValid(bad_cdata.GetQAnode()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TPPACData cdata;
      randomData[i].Copy(cdata);
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX1],cdata.GetTX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTX2],cdata.GetTX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY1],cdata.GetTY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTY2],cdata.GetTY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kTAnode],cdata.GetTAnode());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX1],cdata.GetQX1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQX2],cdata.GetQX2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY1],cdata.GetQY1());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQY2],cdata.GetQY2());
      EXPECT_DOUBLE_EQ(randomDouble[i][kQAnode],cdata.GetQAnode());
      EXPECT_EQ(randomInt[i][0],cdata.GetDetID());
   }
}

TEST_F(TPPACDataTest,Update) {
   for(Int_t i = 0;i!=NDATA;++i) {
      Double_t tx1 = randomDouble[i][kTX1];
      Double_t tx2 = randomDouble[i][kTX2];
      Double_t ty1 = randomDouble[i][kTY1];
      Double_t ty2 = randomDouble[i][kTY2];
      Double_t tanode = randomDouble[i][kTAnode];

      EXPECT_DOUBLE_EQ(tx1 - tx2, randomData[i].GetTXDiff());
      EXPECT_DOUBLE_EQ(tx1 + tx2 - tanode * 2, randomData[i].GetTXSum());
      EXPECT_DOUBLE_EQ(ty1 - ty2, randomData[i].GetTYDiff());
      EXPECT_DOUBLE_EQ(ty1 + ty2 - tanode * 2, randomData[i].GetTYSum());

      Double_t qx1 = randomDouble[i][kQX1];
      Double_t qx2 = randomDouble[i][kQX2];
      Double_t qy1 = randomDouble[i][kQY1];
      Double_t qy2 = randomDouble[i][kQY2];

      EXPECT_DOUBLE_EQ(TMath::Sqrt(qx1 * qx2), randomData[i].GetQXSum());
      EXPECT_DOUBLE_EQ(TMath::Sqrt(qy1 * qy2), randomData[i].GetQYSum());
   }
}

TEST_F(TPPACDataTest,CompareWithInvalid) {
   TPPACData a;
   a.SetTX1(1.);
   a.SetTX2(1.);
   a.SetTY1(1.);
   a.SetTY2(1.);
   a.SetTAnode(1.);
   a.SetQX1(1.);
   a.SetQX2(1.);
   a.SetQY1(1.);
   a.SetQY2(1.);
   a.SetQAnode(1.);
   a.SetDetID(1);

   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}

TEST_F(TPPACDataTest,Operators) {
   for(Int_t i = 0;i!=NDATA;++i) {   
      const Double_t hoge = randomDouble[i][kTAnode];
      EXPECT_DOUBLE_EQ(hoge,(Double_t)randomData[i]);
   }
}
