set(LIBTYPE SHARED)
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
add_subdirectory(cstr)
add_subdirectory(rstr)
add_subdirectory(rpstr)
add_subdirectory(cline)
add_subdirectory(fon)
add_subdirectory(leo)
add_subdirectory(evn)
add_subdirectory(cpage)
add_subdirectory(cimage)
add_subdirectory(cfio)
add_subdirectory(puma)
add_subdirectory(rline)
add_subdirectory(rcutp)
add_subdirectory(dif)
add_subdirectory(msk)
add_subdirectory(rling)
add_subdirectory(ctb)
add_subdirectory(rsadd)
add_subdirectory(std)
add_subdirectory(pass2)
add_subdirectory(rbal)
add_subdirectory(mmx)
add_subdirectory(r35)
add_subdirectory(cpu)
add_subdirectory(rdib)
add_subdirectory(loc)
add_subdirectory(ced)
add_subdirectory(exc)
add_subdirectory(rimage)
add_subdirectory(rverline)
add_subdirectory(rout)
add_subdirectory(rblock)
add_subdirectory(rselstr)
add_subdirectory(rfrmt)
add_subdirectory(rshelllines)
add_subdirectory(lns)
add_subdirectory(rreccom)
add_subdirectory(smetric)
add_subdirectory(rneg)
add_subdirectory(rpic)
add_subdirectory(rstuff)
add_subdirectory(rmarker)
add_subdirectory(rcorrkegl)
add_subdirectory(compat)

set(PUMA_LIBS 
    puma 
    rcorrkegl 
    rmarker 
    rstuff 
    rpic 
    rline 
    rneg 
    smetric 
    rreccom
    cuneiform-lns 
    rshelllines 
    rfrmt 
    rselstr 
    rblock 
    rout 
    rverline 
    rimage 
    cuneiform-exc 
    cuneiform-ced 
    cuneiform-cpage
    cuneiform-cimage 
    loc32 
    rdib 
    cuneiform-cfio
)

install(FILES cuneiform.h DESTINATION include)


