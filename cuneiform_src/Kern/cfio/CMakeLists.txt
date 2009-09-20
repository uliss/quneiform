include_directories(BEFORE)

add_library(cfio ${LIBTYPE}
	ctccontrol.cpp
	ctcfile.cpp
	ctcstorage.cpp
	dll_main.cpp
	ctcglobalheader.cpp
	ctcglobalfile.cpp
	ctcmemoryheader.cpp
	ctcfileheader.cpp
	ctcstorageheader.cpp
	memorylist.cpp
	filelist.cpp
)

target_link_libraries(cfio cfcompat)

install_dll(cfio)
