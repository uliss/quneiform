include_directories(include
h
hh
hhh
hdebug
hrk
hrkint
hrstr
fon/src
#rout
usage
)

# CMake does not currently have direct support for ImageMagick.
include(FindPkgConfig)
pkg_check_modules(MAGICK ImageMagick++)
if(MAGICK_FOUND)
    set(USE_MAGICK TRUE)
    include_directories(${MAGICK_INCLUDE_DIRS})
    link_directories(${MAGICK_LIBRARY_DIRS})
else()
    message(STATUS "ImageMagick++ not found. Only uncompressed BMP images supported.")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_library(DL_LIB dl)

# Emulate Windows DLL symbol resolve order.
if(UNIX AND NOT APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()

include_directories(itigerole/sources/tiger/h)

set(LIBTYPE SHARED CACHE STRING "STATIC or SHARED")

if(LIBTYPE STREQUAL SHARED)
    set(SNP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/hdebug/__snp.c)
    message(STATUS "Adding __snp.c to individual libraries.")
    #add_library(snp SHARED hdebug/__snp.c)
    #set(SNP_LIBS snp)
endif()

add_subdirectory(ccom)
add_subdirectory(cstr)
add_subdirectory(rstr)
add_subdirectory(rpstr)
add_subdirectory(cline)
add_subdirectory(fon)
add_subdirectory(leo)
add_subdirectory(evn32)
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
add_subdirectory(lns32)
add_subdirectory(rreccom)
add_subdirectory(smetric)
add_subdirectory(rneg)
add_subdirectory(rpic)
add_subdirectory(rstuff)
add_subdirectory(rmarker)
add_subdirectory(rcorrkegl)

set(RPSTR_LIBS rpstr rstr rling mmx pass2 rbal cline leo cpu r35 rlings rsadd
fon msk std32 evn32 cstr ctb32 dif rcutp ccom)
set(PUMA_LIBS puma rcorrkegl rmarker rstuff rpic rline rneg smetric rreccom
lns32 rshelllines rfrmt rselstr rblock rout rverline rimage exc ced cpage
cimage loc rdib cfio)

#add_executable(rpstrtest rpstrtest.c windummy.c)
#target_link_libraries(rpstrtest ${RPSTR_LIBS})

add_library(windummy ${LIBTYPE} windummy.c)

add_executable(cuneiform cuneiform-cli.cpp)
target_link_libraries(cuneiform puma ${MAGICK_LIBRARIES} ${DL_LIB})
#${PUMA_LIBS} ${RPSTR_LIBS} ${SNP_LIBS} dl)
