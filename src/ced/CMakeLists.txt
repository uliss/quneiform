add_definitions(-D__CED__)

set(CED_SOURCES
    blockelement.cpp
    cedchar.cpp
    cedcolumn.cpp
    cedframe.cpp
    cedline.cpp
    cedpage.cpp
    cedparagraph.cpp
    cedpicture.cpp
    cedsection.cpp
    cedtable.cpp
    element.cpp
)

set(CED_HEADERS
    ced.h
    cedarchive.h
    cedexporter.h
    cedfwd.h
    cedpageptr.h
)

set(CED_INSTALLED_HEADERS
    cedexporter.h
    cedfwd.h
    cedpageptr.h
)

add_library(cuneiform-ced ${CF_CED_LIBTYPE}
    ${CED_SOURCES}
    ${CED_HEADERS}
)

set(CED_LIBS
    cuneiform-common
    cuneiform-compat
)

if(CF_SERIALIZE)
    list(APPEND CED_LIBS ${Boost_SERIALIZATION_LIBRARY})
endif()

target_link_libraries(cuneiform-ced ${CED_LIBS})

cf_install_library(cuneiform-ced)
cf_install_headers(ced ${CED_INSTALLED_HEADERS})

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-ced UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR)
endif()
