add_library(cpu ${LIBTYPE}
src/cpu.c
src/cpu32.c
)

if(WIN32)
  install(TARGETS cpu ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS cpu LIBRARY DESTINATION ${LIBDIR})
endif()
