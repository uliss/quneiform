set(CF_CPAGE_SOURCES
    backup.cpp
    block.cpp
    cpage.cpp
    cpicture.cpp
    ctablex.cpp
    data.cpp
    defconvert.cpp
    dll_cpage.cpp
    mymem.cpp
    page.cpp
    pagestorage.cpp
    picture.cpp
    table.cpp
    tableclass.cpp
    convert.h
    internal.h
)

add_library(${CF_CPAGE} ${LIBTYPE} ${CF_CPAGE_SOURCES})

set_property(TARGET ${CF_CPAGE} PROPERTY COMPILE_DEFINITIONS __CPAGE__)

target_link_libraries(${CF_CPAGE}
    ${CF_COMPAT}
    ${CF_COMMON}
)

cf_install_library(${CF_CPAGE})
cf_cppcheck(${CF_CPAGE})
