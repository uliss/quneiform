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

add_library(cuneiform-ced ${CF_CED_LIBTYPE}
    ${CED_SOURCES}
    ${CED_HEADERS}
)

set(CED_LIBS
    cuneiform-common
    cuneiform-cfio
    cuneiform-compat
)

if(CF_SERIALIZE)
    list(APPEND CED_LIBS ${Boost_SERIALIZATION_LIBRARY})
endif()

target_link_libraries(cuneiform-ced ${CED_LIBS})

library_hook(cuneiform-ced)

set(CED_HEADER_INSTALL "ced")

install(
    FILES
        cedexporter.h cedfwd.h cedpageptr.h
    DESTINATION
        include/${CF_HEADER_INSTALL}/${CED_HEADER_INSTALL}
)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-ced UNUSED_FUNCTIONS STYLE FAIL_ON_WARNINGS)
endif()
