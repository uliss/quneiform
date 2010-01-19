add_library(cuneiform-cfcompat ${LIBTYPE} 
	cfcompat.cpp
)

set_property(TARGET cuneiform-cfcompat PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

library_hook(cuneiform-cfcompat)
