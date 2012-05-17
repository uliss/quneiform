set(RSTUFF_HEADERS
    rsfunc.h
    rsglobaldata.h
    rstuff_local.h
    rstuff_struct.h
)

set(RSTUFF_SOURCES
    aboutlines.aboutlin.cpp
    aboutlines.buffer.cpp
    binarise.cpp
    componenthistogram.cpp
    deletelines.cpp
    dll.cpp
    layout.cpp
    linekiller.cpp
    normalise.cpp
    ortomove.cpp
    resolutionchecker.cpp
    rsmemory.cpp
    rstimer.cpp
    rstuff.cpp
)

add_library(cuneiform-rstuff ${LIBTYPE}
    ${SNP_SRC}
    ${RSTUFF_SOURCES}
    ${RSTUFF_HEADERS}
)

cf_hide_visibility(cuneiform-rstuff)

target_link_libraries(cuneiform-rstuff
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rdib
    cuneiform-rimage
    cuneiform-rline
    cuneiform-smetric
    cuneiform-rreccom
    cuneiform-exc
    cuneiform-usage
)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-rstuff UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()

cf_install_library(cuneiform-rstuff)
