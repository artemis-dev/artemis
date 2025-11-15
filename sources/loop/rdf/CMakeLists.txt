set(OBJECTS
    TRDFEventStore.cc
)

set(HEADERS
    TRDFEventStore.h
)

art_lib(artrdf "${OBJECTS}" "${HEADERS}"  "artrdf_linkdef.h" "${HEADERS}")
