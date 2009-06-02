add_library(cfcompat ${LIBTYPE} cfcompat.c)
target_link_libraries(cfcompat ${DL_LIB})
set_property(TARGET cfcompat PROPERTY COMPILE_DEFINITIONS __WINDUMMY__)

install_dll(cfcompat)
