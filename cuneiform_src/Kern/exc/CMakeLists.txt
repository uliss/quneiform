include_directories(BEFORE src)

add_definitions(-D__EXC__)

add_library(exc ${LIBTYPE}
src/compdata.c
src/compkit.c
src/extrcomp.c
src/locompmn.c
src/matrix.c
src/v0compan.c
src/v0compgl.c
src/v0compmn.c
)

target_link_libraries(exc loc ccom evn32)

if(WIN32)
  install(TARGETS exc ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS exc LIBRARY DESTINATION ${LIBDIR})
endif()

