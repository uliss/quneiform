include_directories(BEFORE)

add_library(cfio ${LIBTYPE}
	ctccontrol.cpp
	dll.cpp
	globalheader.cpp
	globalfile.cpp
	fileheader.cpp
	filelist.cpp
	memorylist.cpp
	memoryheader.cpp
	storagelist.cpp
	storageheader.cpp
)

target_link_libraries(cfio cfcompat)

install_dll(cfio)
