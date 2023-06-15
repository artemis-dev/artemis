set(OBJECTS
    TParameterArrayLoader.cc
    TParameterLoader.cc
)

set(HEADERS
    TParameterArrayLoader.h
    TParameterLoader.h
)

art_lib(artparam "${OBJECTS}" "${HEADERS}"  "artparam_linkdef.h" ".")
