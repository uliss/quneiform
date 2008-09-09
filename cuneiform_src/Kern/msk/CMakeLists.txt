include_directories(src)

add_library(msk ${LIBTYPE} src/msk.c)

target_link_libraries(msk mmx windummy)

if(WIN32)
  install(TARGETS msk ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS msk LIBRARY DESTINATION ${LIBDIR})
endif()

