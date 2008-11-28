include_directories(BEFORE src)

add_definitions(-D__P2__)

add_library(pass2 ${LIBTYPE}
src/p2_choic.c
src/p2_proc.c
src/p2_rstr.c
src/p2_test.c
src/p2_tools.c
)

target_link_libraries(pass2 cstr fon std32)

if(WIN32)
  install(TARGETS pass2 ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS pass2 LIBRARY DESTINATION ${LIBDIR})
endif()
