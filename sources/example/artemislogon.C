{
   {
      TString path = gSystem->GetDynamicPath();
      path.Append("./processors:$WORKDIR/processors:.");
      gSystem->SetDynamicPath(path);
//      gSystem->Load("libuser");
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
//   cf->Register(TCatCmdLoopAdd::Instance());
//   cf->Register(TCatCmdLoopResume::Instance());
//   cf->Register(TCatCmdLoopSuspend::Instance());
   cf->Register(new TCatCmdHstore);
//   cf->Register(TCatCmdSave::Instance());
//   cf->Register(TCatCmdXval::Instance());

   {
      TString path = gSystem->GetIncludePath();
      path.Append("-I$WORKDIR/processors");
      gSystem->SetIncludePath(path);
   }
   {
      art::TModuleDecoderFactory *df = art::TModuleDecoderFactory::Instance();
      df->Register(new art::TModuleDecoderV1190);
      df->Register(new art::TModuleDecoderV1740);
      df->Register(new art::TModuleDecoderSkip(0));
      df->Register(new art::TModuleDecoderSkip(1));
      df->Register(new art::TModuleDecoderSkip(21));
      df->Register(new art::TModuleDecoderSkip(23));
      df->Register(new art::TModuleDecoderSkip(26));
      
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
