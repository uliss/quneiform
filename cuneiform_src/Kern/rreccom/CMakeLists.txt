include_directories(BEFORE src/h)

add_library(rreccom ${LIBTYPE}
src/cpp/alphabet.cpp
src/cpp/recog.cpp
src/cpp/rreccom.cpp
)