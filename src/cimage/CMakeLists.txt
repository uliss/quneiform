add_library(cuneiform-cimage ${LIBTYPE}
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

set_property(TARGET cuneiform-cimage PROPERTY COMPILE_DEFINITIONS __CIMAGE__)

target_link_libraries(cuneiform-cimage rdib common cuneiform-cfio)

library_hook(cuneiform-cimage)
