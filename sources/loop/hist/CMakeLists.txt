set(OBJECTS
    TAttCut.cc
    TAttTreeProj.cc
    TAxisTreeProj.cc
    TH1FTreeProj.cc
    TH2FTreeProj.cc
    TH3FTreeProj.cc
    TTreeProjection.cc
    TTreeProjectionProcessor.cc
    TTreeProjGroup.cc
)

set(HEADERS
    TTreeProjGroup.h
    TTreeProjectionProcessor.h
    TTreeProjection.h
    TH3FTreeProj.h
    TH2FTreeProj.h
    TH1FTreeProj.h
    TAxisTreeProj.h
    TAttTreeProj.h
    TAttCut.h
)

art_lib(arthist "${OBJECTS}" "${HEADERS}"  "arthist_linkdef.h" "sources/loop/hist")
