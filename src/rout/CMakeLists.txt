include_directories(${ICONV_INCLUDE_DIR})

add_library(rout ${LIBTYPE}
browse.cpp
codetables.cpp
exporter.cpp
exporterfactory.cpp
edexporter.cpp
debugexporter.cpp
rtfexporter.cpp
textexporter.cpp
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

target_link_libraries(rout 
    cuneiform-ced 
    cuneiform-rfrmt)

library_hook(rout)
