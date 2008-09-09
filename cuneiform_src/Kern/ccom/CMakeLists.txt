include_directories(src)

add_library(ccom ${LIBTYPE}
src/ccom.c
src/ccom_loc.c)

if(WIN32)
  install(TARGETS ccom ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS ccom LIBRARY DESTINATION ${LIBDIR})
endif()
