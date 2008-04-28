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

include_directories(itigerole/sources/tiger/h)

set(LIBTYPE STATIC CACHE STRING "STATIC or SHARED")

if(LIBTYPE STREQUAL SHARED)
    set(SNP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/hdebug/__snp.c)
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

add_executable(pumatest pumatest.cpp windummy hdebug/__snp.c)
target_link_libraries(pumatest 

${PUMA_LIBS} ${RPSTR_LIBS})