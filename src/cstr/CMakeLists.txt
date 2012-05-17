add_library(cuneiform-cstr ${LIBTYPE}
cgraph.cpp
cstr.cpp)
set_property(TARGET cuneiform-cstr PROPERTY COMPILE_DEFINITIONS __CSTR__)

target_link_libraries(cuneiform-cstr cuneiform-ccom)

cf_install_library(cuneiform-cstr)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-cstr UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()
