include_directories(BEFORE sources/h sources/cpp)

add_library(cfio ${LIBTYPE}
sources/cpp/ctcbase.cpp
sources/cpp/ctccontrol.cpp
sources/cpp/ctcfile.cpp
sources/cpp/ctcmemory.cpp
sources/cpp/ctcstorage.cpp
sources/cpp/dll_main.cpp
sources/cpp/ctcglobalheader.cpp
)

target_link_libraries(cfio cfcompat)

install_dll(cfio)
