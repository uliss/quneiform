add_definitions(-D__CCOM__) # Sets FUN_IMPO and FUN_EXPO properly.

add_library(cuneiform-ccom ${LIBTYPE}
ccom.cpp
ccom_loc.cpp)

library_hook(cuneiform-ccom)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-ccom UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()
