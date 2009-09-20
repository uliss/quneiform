include_directories(BEFORE)

add_library(cfio ${LIBTYPE}
	ctccontrol.cpp
	ctcfile.cpp
	ctcmemory.cpp
	ctcstorage.cpp
	dll_main.cpp
	ctcglobalheader.cpp
	ctcglobalfile.cpp
	ctcmemoryheader.cpp
	ctcfileheader.cpp
)

target_link_libraries(cfio cfcompat)

install_dll(cfio)
