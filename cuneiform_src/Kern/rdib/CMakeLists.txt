include_directories(BEFORE sources)

add_library(rdib ${LIBTYPE} sources/cpp/CTDIB.cpp)

if(WIN32)
  install(TARGETS rdib ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rdib LIBRARY DESTINATION ${LIBDIR})
endif()
