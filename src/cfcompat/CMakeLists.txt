add_library(cfcompat ${LIBTYPE} 
	cfcompat.cpp
)

set_property(TARGET cfcompat PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

library_hook(cfcompat)
