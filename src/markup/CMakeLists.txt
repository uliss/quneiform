set(MARKUP_SOURCES
    ${SNP_SRC}
    pagemarker.cpp
    shortverticallinesfilter.cpp
    svlprocessor.cpp
)

set(MARKUP_HEADERS
    pagemarker.h
    rmfunc.h
)

add_library(cuneiform-markup ${LIBTYPE}
    ${MARKUP_SOURCES}
    ${MARKUP_HEADERS}
)

cf_hide_visibility(cuneiform-markup)

set_property(TARGET cuneiform-markup PROPERTY COMPILE_DEFINITIONS __RMARKER__)

target_link_libraries(cuneiform-markup
    cuneiform-cimage
    cuneiform-cpage
    cuneiform-rblock
    cuneiform-rline
    cuneiform-rneg
    cuneiform-rpic
    cuneiform-rselstr
    cuneiform-usage
)

cf_install_library(cuneiform-markup)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-markup UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
