set(USAGE_SOURCES
    puma_err.cpp
    un_buff.cpp
)

set(USAGE_HEADERS
    puma_err.h
    un_buff.h
)

add_library(cuneiform-usage ${LIBTYPE}
    ${USAGE_SOURCES}
    ${USAGE_HEADERS}
)
