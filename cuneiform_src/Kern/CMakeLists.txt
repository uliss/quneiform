include_directories(include)
include_directories(h)
include_directories(hh)
include_directories(hhh)
include_directories(hdebug)
include_directories(hrk)
include_directories(hrkint)
include_directories(hrstr)
include_directories(fon/src)


include_directories(itigerole/sources/tiger/h)

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

set(RPSTR_LIBS rpstr rstr rling mmx pass2 rbal cline leo r35 rlings rsadd
fon msk evn32 cstr ctb32 dif rcutp ccom)
set(PUMA_LIBS puma rcorrkegl marker rstuff rpic rline meg smetric rreccom
lns32 rhellines rfmrt rsestr rblock rout rverline rimage exc ced cpage
cimage loc rdib cfio)

add_executable(rpstrtest rpstrtest.c windummy.c)
target_link_libraries(rpstrtest ${RPSTR_LIBS})

#add_executable(pumatest pumatest.cpp windummy hdebug/__snp.c)
#target_link_libraries(pumatest 
#
#puma rline cpage cimage cfio 
#
#${RPSTR_LIBS})