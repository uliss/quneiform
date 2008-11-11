include_directories(BEFORE src/h)

add_library(rcorrkegl ${LIBTYPE}
${SNP_SRC}
src/cpp/cor_kegl.cpp
src/cpp/rcorrkegl.cpp
src/cpp/stdafx.cpp
)

target_link_libraries(rcorrkegl cstr cpage)

if(WIN32)
  install(TARGETS rcorrkegl ARCHIVE DESTINATION ${LIBDIR})
else()
  install(TARGETS rcorrkegl LIBRARY DESTINATION ${LIBDIR})
endif()

