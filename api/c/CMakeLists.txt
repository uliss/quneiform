include_directories(. ../../src ../../src/h)

if(CF_USE_SHMEM)
	add_definitions(-DUSE_SHMEM)
endif()

set(CAPI_SOURCES c_api_def.h c_api.cpp)

add_library(cuneiform-c STATIC
    ${CAPI_SOURCES}
)

target_link_libraries(cuneiform-c cuneiform)

cf_hide_visibility(cuneiform-c)
