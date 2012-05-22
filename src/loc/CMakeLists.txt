set(CF_LOC_SOURCE
    compdata.cpp
    compkit.cpp
    loc.cpp
    locompmn.cpp
    v0compgl.cpp
)

add_library(${CF_LOC} ${LIBTYPE} ${CF_LOC_SOURCE})
cf_hide_visibility(${CF_LOC})
  
set_property(TARGET ${CF_LOC} PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(${CF_LOC} ${CF_COMPAT})
cf_install_library(${CF_LOC})
cf_cppcheck(${CF_LOC})
