add_library(cuneiform-rbal ${LIBTYPE}
gistogramma.cpp
lin_ext.cpp
lin_rec.cpp
linbam.cpp
linban.cpp
linpon.cpp
linutil.cpp
statsearchbl.cpp
)
set_property(TARGET cuneiform-rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(cuneiform-rbal 
    cuneiform-cstr 
    cuneiform-evn)

library_hook(cuneiform-rbal)
