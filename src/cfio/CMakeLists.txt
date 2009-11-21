include_directories(BEFORE)

add_library(cfio ${LIBTYPE}
	control.cpp
	dll.cpp
	fileheader.cpp
	filelist.cpp
	globalheader.cpp
	globalfile.cpp
	memorylist.cpp
	memoryheader.cpp
	storagelist.cpp
	storageheader.cpp
)

target_link_libraries(cfio cfcompat)

install_dll(cfio)
