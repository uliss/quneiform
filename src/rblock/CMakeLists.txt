include_directories(BEFORE ../hhh/tigerh
../hhh/tigerh/h)

set(RBLOCK_HEADERS
    rblock.h
    new_c.h
    newfunc.h
    pic_n_table.h
)

set(RBLOCK_SOURCES
    _adaptive.cpp
    _debug.cpp
    _dll.cpp
    _dlldummy.cpp
    _graphics.cpp
    _loadroots.cpp
    _partlayout.cpp
    _partstring.cpp
    _pic_n_table.cpp
    _rblock.cpp
    compdata.cpp
    ltblglue.cpp
    ltblocks.cpp
    ltbreak.cpp
    lterrors.cpp
    ltexcomp.cpp
    lthand.cpp
    lthlundo.cpp
    lthyst.cpp
    ltibreak.cpp
    ltinclin.cpp
    ltmain.cpp
    ltpage.cpp
    ltremove.cpp
    ltroots.cpp
    ltseps.cpp
    ltsmart.cpp
    lttrees.cpp
    ltwstrip.cpp
    msgerr.cpp
    scalar.cpp
    seblocks.cpp
    seform.cpp
    sehbreak.cpp
    sehyst.cpp
    semain.cpp
    sestring.cpp
    sevbreak.cpp
)

add_library(${CF_RBLOCK} ${LIBTYPE}
    ${SNP_SRC}
    ${RBLOCK_HEADERS}
    ${RBLOCK_SOURCES}
)

cf_hide_visibility(${CF_RBLOCK})

set_property(TARGET ${CF_RBLOCK} PROPERTY COMPILE_DEFINITIONS __RBLOCK__)

target_link_libraries(${CF_RBLOCK}
    ${CF_CPAGE}
    ${CF_LOC}
    ${CF_EXC}
    ${CF_CSTR}
    ${CF_CLINE}
    cuneiform-rstr
    ${CF_EVN}
)

cf_install_library(${CF_RBLOCK})
cf_cppcheck(${CF_RBLOCK})
