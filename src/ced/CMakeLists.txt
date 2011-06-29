add_definitions(-D__CED__)

add_library(cuneiform-ced ${LIBTYPE}
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
	wrapper.cpp
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
install(FILES cedexporter.h DESTINATION include/${CF_HEADER_INSTALL}/${CED_HEADER_INSTALL})
