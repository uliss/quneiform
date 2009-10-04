include_directories(src)

add_library(rbal ${LIBTYPE}
gistogramma.cpp
lin_ext.cpp
lin_rec.cpp
linbam.cpp
linban.cpp
linpon.cpp
linutil.cpp
statsearchbl.cpp
)
set_property(TARGET rbal PROPERTY COMPILE_DEFINITIONS __RBAL__)

target_link_libraries(rbal cstr evn32)

install_dll(rbal)
