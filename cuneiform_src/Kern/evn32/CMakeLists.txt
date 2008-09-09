include_directories(src)

add_library(evn32 ${LIBTYPE}
src/compdata.c
src/compkit.c
src/evn.c
src/locompmn.c
src/rastror.c
src/v0compan.c
src/v0comper.c
src/v0compev.c
src/v0compgl.c
src/v0comprq.c
)

target_link_libraries(evn32 dif windummy)

if(WIN32)
  install(TARGETS evn32 ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS evn32 LIBRARY DESTINATION ${LIBDIR})
endif()

