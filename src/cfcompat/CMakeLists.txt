add_library(cuneiform-compat ${LIBTYPE} 
	cfcompat.cpp
)

set_property(TARGET cuneiform-compat PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

library_hook(cuneiform-compat)
