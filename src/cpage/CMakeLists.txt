add_library(cpage ${LIBTYPE}
backup.cpp
block.cpp
compress.cpp
cpage.cpp
cpicture.cpp
ctablex.cpp
data.cpp
defconvert.cpp
dll_cpage.cpp
mymem.cpp
namedata.cpp
page.cpp
picture.cpp
table.cpp
tableclass.cpp
)
set_property(TARGET cpage PROPERTY COMPILE_DEFINITIONS __CPAGE__)

target_link_libraries(cpage cuneiform-cfcompat)

library_hook(cpage)
