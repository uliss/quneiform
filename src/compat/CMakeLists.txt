add_library(cuneiform-compat ${LIBTYPE} 
	cfcompat.cpp
)

set_property(TARGET cuneiform-compat PROPERTY COMPILE_DEFINITIONS __CFCOMPAT__)

library_hook(cuneiform-compat)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-compat UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()
