include_directories(BEFORE src)
add_definitions(-D__MMX__)

add_library(mmx ${LIBTYPE}
src/mmx_gra.c
src/mmx_ind.c
src/mmx_net.c
src/mmx_r35.c
src/mmxmain.c
)

if(WIN32)
  install(TARGETS mmx ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS mmx LIBRARY DESTINATION ${LIBDIR})
endif()
