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

target_link_libraries(rbal cstr evn32)