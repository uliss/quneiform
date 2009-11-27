add_library(cimage ${LIBTYPE}
${SNP_SRC}
cticontrol.cpp
ctiimage.cpp
ctiimageheader.cpp
ctiimagelist.cpp
ctimask.cpp
ctimaskline.cpp
ctimasklinesegment.cpp
ctimemory.cpp
dll.cpp
)

set_property(TARGET cimage PROPERTY COMPILE_DEFINITIONS __CIMAGE__)

target_link_libraries(cimage cfio rdib)

install_dll(cimage)
