add_definitions(-D__CED__)

set(CF_CED_SOURCES
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

set(CF_CED_HEADERS
    ced.h
    cedarchive.h
    cedexporter.h
    cedfwd.h
    cedpageptr.h
)

set(CF_CED_API
    cedexporter.h
    cedfwd.h
    cedpageptr.h
)

add_library(cuneiform-ced ${CF_CED_LIBTYPE}
    ${CF_CED_SOURCES}
    ${CF_CED_HEADERS}
)

set(CF_CED_LINK
    ${CF_COMMON}
    ${CF_COMPAT}
)

if(CF_SERIALIZE)
    list(APPEND CF_CED_LINK ${Boost_SERIALIZATION_LIBRARY})
endif()

target_link_libraries(${CF_CED} ${CF_CED_LINK})

cf_install_library(${CF_CED})
cf_install_headers(ced ${CF_CED_API})
cf_cppcheck(${CF_CED})
