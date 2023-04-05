set(OBJECTS
    TGetEventStore.cc
)

set(HEADERS
    TGetEventStore.h
)

art_lib(artget "${OBJECTS}" "${HEADERS}"  "artget_linkdef.h" "sources/loop/get")