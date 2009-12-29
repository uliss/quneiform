add_library(rdib ${LIBTYPE} 
ctdib.cpp
bmpimageloader.cpp
image.cpp
imageloader.cpp
imageloaderfactory.cpp
magickimageloader.cpp
)
set_property(TARGET rdib PROPERTY COMPILE_DEFINITIONS __RDIB__)

install_dll(rdib)
