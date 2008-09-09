include_directories(BEFORE sources/h sources/cpp)

add_library(cfio ${LIBTYPE}
sources/cpp/ctcbase.cpp
sources/cpp/ctccontrol.cpp
sources/cpp/ctcfile.cpp
sources/cpp/ctcmemory.cpp
sources/cpp/ctcstorage.cpp
sources/cpp/dll_main.cpp
)

target_link_libraries(cfio windummy)

if(WIN32)
  install(TARGETS cfio ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS cfio LIBRARY DESTINATION ${LIBDIR})
endif()
