add_library(cuneiform-cpage ${LIBTYPE}
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
set_property(TARGET cuneiform-cpage PROPERTY COMPILE_DEFINITIONS __CPAGE__)

target_link_libraries(cuneiform-cpage 
    cuneiform-compat
)

cf_install_library(cuneiform-cpage)

if(TESTS)
    include(CppcheckTargets)
    add_cppcheck(cuneiform-cpage UNUSED_FUNCTIONS STYLE POSSIBLE_ERROR FORCE QUIET)
endif()
