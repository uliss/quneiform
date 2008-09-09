add_library(r35 ${LIBTYPE}
src/r35.c)

target_link_libraries(r35 mmx std32 windummy)

if(WIN32)
  install(TARGETS r35 ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS r35 LIBRARY DESTINATION ${LIBDIR})
endif()

