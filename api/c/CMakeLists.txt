include_directories(. ../../src ../../src/h)

set(CAPI_SOURCES c_api_def.h c_api.cpp)

add_library(cuneiform-c STATIC
    ${CAPI_SOURCES}
)

cif_visibility_hidden(cuneiform-c)
