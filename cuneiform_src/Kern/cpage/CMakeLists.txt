include_directories(BEFORE sources/cpp sources/main)

add_library(cpage ${LIBTYPE}
sources/cpp/backup.cpp
sources/cpp/block.cpp
sources/cpp/compress.cpp
sources/main/cpage.cpp
#sources/main/cpage.rc
sources/cpp/cpicture.cpp
sources/main/ctablex.cpp
sources/cpp/data.cpp
sources/cpp/defconvert.cpp
sources/main/dll_cpage.cpp
sources/main/mymem.cpp
sources/cpp/namedata.cpp
sources/cpp/page.cpp
sources/cpp/picture.cpp
sources/cpp/table.cpp
sources/cpp/tableclass.cpp
)
set_property(TARGET cpage PROPERTY COMPILE_DEFINITIONS __CPAGE__)

target_link_libraries(cpage cfcompat)

install_dll(cpage)
