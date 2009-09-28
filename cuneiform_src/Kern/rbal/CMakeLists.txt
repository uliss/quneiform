include_directories(src)

add_library(rbal ${LIBTYPE}
src/gistogramma.cpp
src/lin_ext.cpp
src/lin_rec.cpp
src/linbam.cpp
src/linban.cpp
src/linpon.cpp
src/linutil.cpp
src/statsearchbl.cpp
)
set_property(TARGET rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(rbal cstr evn32)

install_dll(rbal)
