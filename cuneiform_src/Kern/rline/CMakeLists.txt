include_directories(BEFORE sources)

add_library(rline ${LIBTYPE}
sources/dll.cpp
sources/findlostlines.cpp
sources/newline.cpp
sources/rline.cpp
)

target_link_libraries(rline
cimage
cpage
lns32
rshelllines
)

if(WIN32)
  install(TARGETS rline ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rline LIBRARY DESTINATION ${LIBDIR})
endif()

