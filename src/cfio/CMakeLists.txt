add_library(cuneiform-cfio ${LIBTYPE}
ctcbase.cpp
ctccontrol.cpp
ctcfile.cpp
ctcmemory.cpp
ctcstorage.cpp
dll_main.cpp
ctcglobalheader.cpp
)

target_link_libraries(cuneiform-cfio cuneiform-compat)

library_hook(cuneiform-cfio)
