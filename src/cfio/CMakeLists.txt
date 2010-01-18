add_library(cfio ${LIBTYPE}
ctcbase.cpp
ctccontrol.cpp
ctcfile.cpp
ctcmemory.cpp
ctcstorage.cpp
dll_main.cpp
ctcglobalheader.cpp
)

target_link_libraries(cfio cfcompat)

library_hook(cfio)
