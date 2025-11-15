#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class art::TArtemisObject+;
#pragma link C++ class art::TDataObject+;
#pragma link C++ class art::TCatParameterTable;
#pragma link C++ class art::TRunInfo+;
#pragma link C++ class TCatHistManager+;
#pragma link C++ class TCatPadManager;
#pragma link C++ class art::TConfigFile;
#pragma link C++ class art::TSharedMemory;
#pragma link C++ class art::TSemaphoreObject;
#pragma link C++ class art::TArtCanvas;
#pragma link C++ class art::TAnalysisInfo-;
#pragma link C++ class art::TMacroReplacer-;
#pragma link C++ namespace art::Util;
#pragma link C++ namespace TArtSystemOfUnit;
#if HAVE_ZMQ_H
#pragma link C++ class art::TZmqSubscriber;
#endif
#endif /* __CINT__ */
