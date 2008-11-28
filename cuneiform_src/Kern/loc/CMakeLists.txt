include_directories(BEFORE src)

add_definitions(-D__LOC__)
add_library(loc ${LIBTYPE}
src/compdata.c
src/compkit.c
src/loc.c
src/locompmn.c
src/v0compgl.c
)

target_link_libraries(loc windummy)

if(WIN32)
  install(TARGETS loc ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS loc LIBRARY DESTINATION ${LIBDIR})
endif()

