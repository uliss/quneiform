add_definitions(-D__CED__)

add_library(cuneiform-ced ${LIBTYPE}
blockelement.cpp
cedchar.cpp
cedline.cpp
cedpage.cpp
cedparagraph.cpp
cedpicture.cpp
cedsection.cpp
cedtable.cpp
ced_func_rtf.cpp
dll.cpp
element.cpp
wrapper.cpp
)

target_link_libraries(cuneiform-ced 
	cuneiform-common 
    cuneiform-cfio
    cuneiform-compat)

library_hook(cuneiform-ced)
