if(USE_ICONV)
    include_directories(${ICONV_INCLUDE_DIR})
endif()

add_library(cuneiform-rout ${LIBTYPE}
browse.cpp
codetables.cpp
exporter.cpp
exporterfactory.cpp
edexporter.cpp
debugexporter.cpp
genericexporter.cpp
htmlexporter.cpp
imageexporter.cpp
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

set_property(TARGET cuneiform-rout PROPERTY COMPILE_DEFINITIONS __ROUT__)

target_link_libraries(cuneiform-rout 
    cuneiform-ced 
    cuneiform-rfrmt)

library_hook(cuneiform-rout)
