include_directories(sources/src/h)

add_library(cline
sources/src/main/clinebase.cpp
sources/src/cpp/clinefunc.cpp
sources/src/main/clinemain.cpp
sources/src/cpp/debug.cpp
sources/src/cpp/lcline.cpp
)
