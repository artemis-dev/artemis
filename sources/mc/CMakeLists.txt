set(OBJECTS
	TArtEventCollections.cc
	TArtParticle.cc
	TArtAtomicMassTable.cc
	TCatTwoBodyKinematics.cc
	TBinaryReactionGenerator.cc
	TTrackG4.cc
	TRandomGraph.cc 
)

set(HEADERS
    TTrackG4.h
    TRandomGraph.h
    TCatTwoBodyKinematics.h
    TBinaryReactionGenerator.h
    TArtParticle.h
    TArtEventCollections.h
    TArtAtomicMassTable.h
)

art_lib(catmc "${OBJECTS}" "${HEADERS}"  "catmc_linkdef.h" "mc")