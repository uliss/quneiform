add_library(cfcompat ${LIBTYPE} 
	cfcompat.cpp
)

target_link_libraries(cfcompat ${DL_LIB})
set_property(TARGET cfcompat PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

install_dll(cfcompat)
