include_directories(BEFORE sources/main)

add_library(cimage ${LIBTYPE}
sources/main/cticontrol.cpp
sources/main/ctiimage.cpp
#sources/main/ctiimage.RC
sources/main/ctiimageheader.cpp
sources/main/ctiimagelist.cpp
sources/main/ctimask.cpp
sources/main/ctimaskline.cpp
sources/main/ctimasklinesegment.cpp
sources/main/ctimemory.cpp
sources/main/dll.cpp
)