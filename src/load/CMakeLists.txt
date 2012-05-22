set(CF_LOAD_SOURCES)
set(CF_LOAD_HEADERS)
set(CF_LOAD_LIBS
    ${CF_COMON}
    ${CF_COMPAT}
    ${CF_CED}
)

if(CF_SERIALIZE)
    list(APPEND CF_LOAD_SOURCES
        cuneiformbinloader.cpp
        cuneiformtextloader.cpp
    )
    list(APPEND CF_LOAD_LIBS ${Boost_SERIALIZATION_LIBRARY})
    message(STATUS "Loaders: Cuneiform Format loaders will be build")

    add_library(${CF_LOAD} ${CF_LOAD_LIBTYPE}
        ${CF_LOAD_SOURCES}
        ${CF_LOAD_HEADERS}
    )
    
    target_link_libraries(${CF_LOAD} ${CF_LOAD_LIBS})
    cf_install_library(${CF_LOAD})
    cf_cppcheck(${CF_LOAD})
endif()

