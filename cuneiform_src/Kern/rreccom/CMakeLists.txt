include_directories(BEFORE src/h)

add_library(rreccom ${LIBTYPE}
src/cpp/alphabet.cpp
src/cpp/recog.cpp
src/cpp/rreccom.cpp
)

target_link_libraries(rreccom ccom evn32 windummy)

install(TARGETS rreccom LIBRARY DESTINATION lib)
