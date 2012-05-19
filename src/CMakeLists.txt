if(NOT LIBTYPE)
    set(LIBTYPE SHARED)
endif()

set(NEWLIBTYPE STATIC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    include
    h
    hh
    hhh
    hdebug
    hrstr
    usage
)

if(LIBTYPE STREQUAL SHARED)
    set(SNP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/hdebug/__snp.cpp)
    message(STATUS "Adding __snp.cpp to individual libraries.")
else()
    add_subdirectory(hdebug)
endif()

set(CF_ALPHABETS cuneiform-alphabets)
set(CF_CCOM cuneiform-ccom)
set(CF_CED cuneiform-ced)
set(CF_CIMAGE cuneiform-cimage)
set(CF_COMMON cuneiform-common)
set(CF_COMPAT cuneiform-compat)
set(CF_CLINE cuneiform-cline)
set(CF_CPAGE cuneiform-cpage)
set(CF_CSTR cuneiform-cstr)
set(CF_CTB cuneiform-ctb)
set(CF_DIF cuneiform-dif)
set(CF_EVN cuneiform-evn)
set(CF_EXC cuneiform-exc)
set(CF_FON cuneiform-fon)
set(CF_LEO cuneiform-leo)
set(CF_LNS cuneiform-lns)

add_subdirectory(common)
add_subdirectory(alphabets)
add_subdirectory(ccom)
add_subdirectory(ced)
add_subdirectory(cimage)
add_subdirectory(cline)

add_subdirectory(compat)
add_subdirectory(cpage)
add_subdirectory(cpu)
add_subdirectory(cstr)
add_subdirectory(ctb)
add_subdirectory(dif)
add_subdirectory(evn)
add_subdirectory(exc)
add_subdirectory(export)
add_subdirectory(fon)
add_subdirectory(rstr)
add_subdirectory(rpstr)
add_subdirectory(leo)
add_subdirectory(lns)
add_subdirectory(load)
add_subdirectory(loc)
add_subdirectory(markup)
add_subdirectory(mmx)
add_subdirectory(msk)
add_subdirectory(pass2)
add_subdirectory(puma)
add_subdirectory(r35)
add_subdirectory(rbal)
add_subdirectory(rcutp)
add_subdirectory(rdib)
add_subdirectory(rline)
add_subdirectory(rling)
add_subdirectory(rsadd)
add_subdirectory(rimage)
add_subdirectory(rverline)
add_subdirectory(rblock)
add_subdirectory(rselstr)
add_subdirectory(rfrmt)
add_subdirectory(rshelllines)
add_subdirectory(rreccom)
add_subdirectory(rneg)
add_subdirectory(rpic)
add_subdirectory(rstuff)
add_subdirectory(rcorrkegl)
add_subdirectory(smetric)
add_subdirectory(std)

if(CF_USE_SHMEM)
    add_subdirectory(shmem)
endif()

add_subdirectory(usage)

set(CF_GLOBAL_HEADERS
    h/globus.h
    h/specprj.h
    h/cttypes.h
    h/err16.h
    cuneiform.h 
    include/cfcompat.h 
    include/compat_defs.h
    include/minmax.h
    include/filestuff.h
    ${CMAKE_BINARY_DIR}/src/include/config.h
    ${CMAKE_BINARY_DIR}/src/include/config-user.h
    ${CMAKE_BINARY_DIR}/src/include/config-serialize.h
)
    
cf_install_headers("" ${CF_GLOBAL_HEADERS})


