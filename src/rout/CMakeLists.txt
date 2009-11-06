add_library(rout ${LIBTYPE}
browse.cpp
codetables.cpp
debugexporter.cpp
exporter.cpp
html.cpp
pictures.cpp
rout.cpp
rout_dll.cpp
tabledbf.cpp
tabletext.cpp
text.cpp
words.cpp
)
set_property(TARGET rout PROPERTY COMPILE_DEFINITIONS __ROUT__)

target_link_libraries(rout ced cfio)

install_dll(rout)
