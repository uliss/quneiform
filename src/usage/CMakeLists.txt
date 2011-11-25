set(USAGE_SOURCES
    ${SNP_SRC}
    puma_err.cpp
    un_buff.cpp
    puma_comm.cpp
)

set(USAGE_HEADERS
    puma_err.h
    un_buff.h
)

add_library(cuneiform-usage ${LIBTYPE}
    ${USAGE_SOURCES}
    ${USAGE_HEADERS}
)
