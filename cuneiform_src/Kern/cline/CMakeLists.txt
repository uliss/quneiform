include_directories(sources/src/h)

add_library(cline ${LIBTYPE}
${SNP_SRC}
sources/src/main/clinebase.cpp
sources/src/cpp/clinefunc.cpp
sources/src/main/clinemain.cpp
sources/src/cpp/debug.cpp
sources/src/cpp/lcline.cpp
)

target_link_libraries(cline windummy)

install(TARGETS cline LIBRARY DESTINATION lib)
