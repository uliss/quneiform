add_library(cuneiform-loc ${LIBTYPE}
compdata.cpp
compkit.cpp
loc.cpp
locompmn.cpp
v0compgl.cpp
)        

cf_hide_visibility(cuneiform-loc)
  
set_property(TARGET cuneiform-loc PROPERTY COMPILE_DEFINITIONS __LOC__)

target_link_libraries(cuneiform-loc cuneiform-compat)

cf_install_library(cuneiform-loc)
