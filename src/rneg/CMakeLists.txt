set(CF_RNEG_SOURCE
    negatestcell.cpp
    negdefs.cpp
    negrecog.cpp
    recnegmain.cpp
    rnegbase.cpp
    searchneg.cpp
)

add_library(${CF_RNEG} ${LIBTYPE}
    ${SNP_SRC}
    ${CF_RNEG_SOURCE}
)
set_property(TARGET ${CF_RNEG} PROPERTY COMPILE_DEFINITIONS __RNEG__)

target_link_libraries(${CF_RNEG}
    ${CF_CIMAGE}
    ${CF_CPAGE}
    ${CF_RDIB}
    ${CF_RIMAGE}
    ${CF_RSELSTR}
    ${CF_EXC}
    ${CF_RSTR}
)

cf_install_library(${CF_RNEG})
cf_cppcheck(${CF_RNEG})
