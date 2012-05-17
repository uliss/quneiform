set(LOAD_SOURCES)
set(LOAD_HEADERS)
set(LOAD_LIBS
    cuneiform-common
    cuneiform-compat
    cuneiform-ced
)

if(CF_SERIALIZE)
    list(APPEND LOAD_SOURCES
        cuneiformbinloader.cpp
        cuneiformtextloader.cpp
    )
    list(APPEND LOAD_LIBS ${Boost_SERIALIZATION_LIBRARY})
    message(STATUS "Loaders: Cuneiform Format loaders will be build")

    add_library(cuneiform-load ${CF_LOAD_LIBTYPE}
        ${LOAD_SOURCES}
        ${LOAD_HEADERS}
    )
    
    target_link_libraries(cuneiform-load ${LOAD_LIBS})
    cf_install_library(cuneiform-load)
endif()

