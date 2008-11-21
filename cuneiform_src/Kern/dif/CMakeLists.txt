include_directories(BEFORE src)
add_definitions(-D__DIF__)

add_library(dif ${LIBTYPE}
src/dif.c
src/diffr.c
src/diffrb.c
src/sticchar.c
src/sticdif.c
src/stictool.c
)

if(WIN32)
  install(TARGETS dif ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS dif LIBRARY DESTINATION ${LIBDIR})
endif()

