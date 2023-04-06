set(OBJECTS
    TRIDFEventStore.cc
)

set(HEADERS
    TRIDFEventStore.h
)

art_lib(artridf "${OBJECTS}" "${HEADERS}"  "artridf_linkdef.h" "sources/loop/ridf")