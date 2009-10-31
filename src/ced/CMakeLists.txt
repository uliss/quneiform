add_definitions(-D__CED__)

add_library(ced ${LIBTYPE}
ced.cpp
cedchar.cpp
cedline.cpp
cedpage.cpp
cedparagraph.cpp
cedsection.cpp
ced_func.cpp
ced_func_old.cpp
ced_func_rtf.cpp
dll.cpp
wrapper.cpp
)

target_link_libraries(ced cfio cfcompat)

install_dll(ced)
