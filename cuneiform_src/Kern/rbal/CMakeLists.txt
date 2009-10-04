include_directories(src)

add_library(rbal ${LIBTYPE}
gistogramma.cpp
lin_ext.c
lin_rec.c
linbam.c
linban.c
linpon.c
linutil.c
statsearchbl.cpp
)
set_property(TARGET rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(rbal cstr evn32)

install_dll(rbal)
