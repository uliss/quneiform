include_directories(src)

add_library(rbal ${LIBTYPE}
src/gistogramma.cpp
src/lin_ext.c
src/lin_rec.c
src/linbam.c
src/linban.c
src/linpon.c
src/linutil.c
src/statsearchbl.cpp
)
set_property(TARGET rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(rbal cstr evn32)

install_dll(rbal)
