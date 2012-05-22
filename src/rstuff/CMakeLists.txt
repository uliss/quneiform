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

add_library(${CF_RSTUFF} ${LIBTYPE}
    ${SNP_SRC}
    ${RSTUFF_SOURCES}
    ${RSTUFF_HEADERS}
)

cf_hide_visibility(${CF_RSTUFF})

target_link_libraries(${CF_RSTUFF}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_RDIB}
    ${CF_RIMAGE}
    ${CF_RLINE}
    ${CF_SMETRIC}
    ${CF_RRECCOM}
    ${CF_EXC}
    ${CF_USAGE}
)

cf_install_library(${CF_RSTUFF})
cf_cppcheck(${CF_RSTUFF})
