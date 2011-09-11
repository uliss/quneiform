set(LOAD_SOURCES)
set(LOAD_HEADERS)
set(LOAD_LIBS
    cuneiform-common
    cuneiform-compat
    cuneiform-cfio
    cuneiform-ced
)

if(Boost_SERIALIZATION_FOUND)
    list(APPEND LOAD_SOURCES
        cuneiformbinloader.cpp
        cuneiformtextloader.cpp
    )
    list(APPEND LOAD_LIBS ${Boost_SERIALIZATION_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
    message(STATUS "Loaders: Cuneiform Format loaders will be build")
endif()

add_library(cuneiform-load ${CF_LOAD_LIBTYPE}
    ${LOAD_SOURCES}
    ${LOAD_HEADERS}
)

target_link_libraries(cuneiform-load ${LOAD_LIBS})
library_hook(cuneiform-load)


