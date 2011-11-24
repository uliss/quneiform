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

add_subdirectory(common)
add_subdirectory(alphabets)
add_subdirectory(ccom)
add_subdirectory(ced)
add_subdirectory(cfio)
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
add_subdirectory(shmem)

install(FILES 
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
    DESTINATION include/${CF_HEADER_INSTALL})


