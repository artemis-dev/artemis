{
   if (0) {
      TString path = gSystem->GetDynamicPath();
      path.Append(":./processors:.");
      gSystem->SetDynamicPath(path);
      gSystem->Load("libuser");
//      gSystem->Load("libMinuit");
   }

   TCatCmdFactory *cf = TCatCmdFactory::Instance();
   cf->SetOptExactName(kFALSE);
   cf->Register(TCatCmdHelp::Instance());
   cf->Register(TCatCmdHt::Instance());
   cf->Register(TCatCmdHtp::Instance());
   cf->Register(TCatCmdHb::Instance());
   cf->Register(TCatCmdHn::Instance());
   cf->Register(TCatCmdZone::Instance());
   cf->Register(TCatCmdLs::Instance());
   cf->Register(TCatCmdCd::Instance());
   cf->Register(TCatCmdPrx::Instance());
   cf->Register(TCatCmdPry::Instance());
//   cf->Register(TCatCmdMnx::Instance());
//   cf->Register(TCatCmdMny::Instance());
//   cf->Register(TCatCmdAvx::Instance());
//   cf->Register(TCatCmdAvy::Instance());
//   cf->Register(TCatCmdBnx::Instance());
//   cf->Register(TCatCmdBny::Instance());
   cf->Register(TCatCmdSly::Instance());
   cf->Register(TCatCmdLoopAdd::Instance());
   cf->Register(TCatCmdLoopResume::Instance());
   cf->Register(TCatCmdLoopSuspend::Instance());
   cf->Register(TCatCmdLoopTerminate::Instance());
   cf->Register(new TCatCmdHstore);
   cf->Register(TCatCmdSave::Instance());
   cf->Register(TCatCmdXval::Instance());
   cf->Register(new art::TCmdFileLs);
   cf->Register(new art::TCmdFileCd);
   cf->Register(new art::TCmdBranchInfo);
   cf->Register(new art::TCmdClassInfo);
   cf->Register(new art::TCmdProcessorDescription);
   
   {
      TString path = gSystem->GetIncludePath();
      path.Append("-I./processors");
      gSystem->SetIncludePath(path);
   }
   {
      art::TModuleDecoderFactory *df = art::TModuleDecoderFactory::Instance();
// mod ID 0 : Fixed16
      const Int_t digits0 = 16;
      df->Register(new art::TModuleDecoderFixed<unsigned short>(0,digits0) );
// mod ID 1 : Fixed24
      const Int_t digits1 = 24;
      df->Register(new art::TModuleDecoderFixed<unsigned int>(1,digits1) );
// mod ID 21 : V7XX
      df->Register(new art::TModuleDecoderV7XX);
// mod ID 23 : V767
      df->Register(new art::TModuleDecoderV767);
// mod ID 24 : V1190
      df->Register(new art::TModuleDecoderV1190);
// mod ID 26 : V1190C
      df->Register(new art::TModuleDecoderSkip(26));
// mod ID 63 : V1740
      df->Register(new art::TModuleDecoderV1740);
      
//      TArtDecoderFactory *df = TArtDecoderFactory::Instance();
//      df->Register(TArtDecoderSIS3610::Instance());
//      df->Register(TArtDecoderSIS3820::Instance());
////      TArtDecoderV1190 *dc = (TArtDecoderV1190*)TArtDecoderV1190::Instance();
////      dc->SetTrailingChOffset(128);
////      df->Register(TArtDecoderV1190::Instance());
//      df->Register(new TCatDecoderV1190);
//      df->Register(TArtDecoderFixed16::Instance());
//      df->Register(TArtDecoderV7XX::Instance());
////      df->Register(TCatDecoderV1740::Instance());
   }
}
